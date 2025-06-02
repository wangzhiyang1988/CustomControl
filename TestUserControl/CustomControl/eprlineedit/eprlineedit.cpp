#include "eprlineedit.h"
#include "ui_eprlineedit.h"
EprLineEdit::EprLineEdit(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::EprLineEditClass)
{
    ui->setupUi(this);

    lineEdit()->setStyleSheet(R"(
 QLineEdit:disabled{
border:1px solid #E1E1E1;
background-color: #F6F6F6;;
padding-left:3px; 

}

QLineEdit{

border:1px solid #C6C6C6;
background-color: #FFFFFF;
padding-left:3px;

}
QLineEdit:hover{

background: #FFFFFF;
border: 1px solid #2453A4;
padding-left:3px;

})");
    connect(lineEdit(), &QLineEdit::editingFinished, this, &EprLineEdit::editingFinished);
    // 文本改变事件不立即触发信号
    // 当失去焦点 或回车 或上下键才进行处理valueChanged
    // connect(lineEdit(), &QLineEdit::textChanged, this, &EprLineEdit::onTextChanged);

    this->setFocusPolicy(Qt::ClickFocus);

    // 注册事件，屏蔽Tab键
    this->installEventFilter(this);
    ui->input->installEventFilter(this);
    ui->btnAdd->installEventFilter(this);
    ui->btnSub->installEventFilter(this);
    ui->input->setProperty("isNormalLineEdit", "yes");
    //
    hideButton();
}

EprLineEdit::EprLineEdit(const QString& text, QWidget* parent)
    : EprLineEdit(parent)
{
    setText(text);
}
void EprLineEdit::setValidator(const QValidator* validator)
{
    lineEdit()->setValidator(validator);
    //
    if (const auto* doubleValidator = dynamic_cast<const Epr100DoubleValidator*>(validator)) {
        setDecimals(doubleValidator->decimals());
        setMinMaxRange(doubleValidator->bottom(), doubleValidator->top());
    }
    else if (const auto* intValidator = dynamic_cast<const Epr100IntValidator*>(validator)) {
        setDecimals(0);
        setStep(1);
        setMinMaxRange(intValidator->bottom(), intValidator->top());
    }
}

QString EprLineEdit::text() const
{
    // return QString::number(value(), 'f', ui->input->decimals());
    return ui->input->text();
}

bool EprLineEdit::setText(const QString& text, bool notify)
{
    bool isOk          = false;
    const double value = text.toDouble(&isOk);
    if (!isOk) {
        // ui->input->setText(text);
        return isOk;
    }
    setValue(value, notify);
    return true;
}

void EprLineEdit::setReadOnly(bool flag)
{
    lineEdit()->setReadOnly(flag);
}

bool EprLineEdit::isReadOnly()
{
    return lineEdit()->isReadOnly();
}

void EprLineEdit::setAlignment(Qt::Alignment flag)
{
    lineEdit()->setAlignment(flag);
}

void EprLineEdit::setMaxLength(int len)
{
    lineEdit()->setMaxLength(len);
    double value = 9;
    for (int i = 0; i < len; ++i) {
        value = value * 10 + 9;
    }
    _maxValue = value;
}

void EprLineEdit::setPlaceholderText(const QString& text)
{
    lineEdit()->setPlaceholderText(text);
}

void EprLineEdit::setStep(double step)
{
    if (step >= _maxValue) {
        step = _maxValue / 10;
    }
    this->_step = step;
}

void EprLineEdit::setValue(double value, bool notify)
{
    _notify               = notify; // 关闭触发valueChange条件
    const double val      = qMax(qMin(value, _maxValue), _minValue);
    const QString newText = QString::number(val, 'f', _decimals);
    if (text() == newText) {
        return;
    }
    ui->input->setText(newText);
    _notify = true; // 解锁触发信号条件
}

double EprLineEdit::value()
{
    return ui->input->text().toDouble();
}

void EprLineEdit::setThrottle(bool enable)
{
    this->_enableThrottle = enable;
}

void EprLineEdit::setMinMaxRange(double min, double max)
{
    _notify = false;

    if (qAbs(min - max) < 1e-6) {
        min = max - 1;
    }

    _maxValue = qMax(min, max);
    _minValue = qMin(min, max);

    // 根据最值更新当前值
    double val = value();
    val        = qMax(qMin(val, _maxValue), _minValue);

    // ui->input->setValue(val);
    setValue(val, _notify);

    _notify = true;
}

void EprLineEdit::setTimerDelay(int _delay)
{
    this->delay = _delay;
}

void EprLineEdit::setEnable(bool _enable)
{
    ui->input->setEnabled(_enable);
    ui->btnAdd->setEnabled(_enable);
    ui->btnSub->setEnabled(_enable);
}

void EprLineEdit::setDecimals(int decimals)
{
    _decimals = decimals;
}

int EprLineEdit::decimals() const
{
    return _decimals;
}

void EprLineEdit::hideButton() const
{
    ui->btnAdd->hide();
    ui->btnSub->hide();
    ui->input->setAlignment(Qt::AlignLeft);
}

QLineEdit* EprLineEdit::lineEdit()
{
    return ui->input;
}

double EprLineEdit::getMaxValue() const
{
    return _maxValue;
}

double EprLineEdit::getMinValue() const
{
    return _minValue;
}

bool EprLineEdit::eventFilter(QObject* obj, QEvent* e)
{
    if (e->type() == QEvent::KeyPress) {
        const QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(e);
        if (keyEvent && (keyEvent->key() == Qt::Key_Tab)) {
            return true;
        }
        else {
            return QWidget::eventFilter(obj, e); // 不影响其它事件操作
        }
    }
    else {
        return QWidget::eventFilter(obj, e); // 不影响其它事件操作
    }
}

void EprLineEdit::clearFocus()
{
    ui->input->clearFocus();
    return QWidget::clearFocus();
}
EprLineEdit::~EprLineEdit()
{
    delete ui;
}

void EprLineEdit::on_btnSub_clicked()
{
    double curValue = value();
    curValue -= _step;
    curValue = qMax(curValue, _minValue);
    setValue(curValue);
}

void EprLineEdit::on_btnAdd_clicked()
{
    double curValue = value();
    curValue += _step;
    curValue = qMin(curValue, _maxValue);
    setValue(curValue);
}

void EprLineEdit::onTextChanged(const QString& text)
{
    if (!_notify) {
        return;
    }
    double arg1 = text.toDouble();
    double val  = qMax(qMin(arg1, _maxValue), _minValue);
    // 是否节流
    static double currentValue;
    currentValue              = val;
    const QString currentText = QString::number(currentValue, 'f', decimals());
    // 不执行节流
    if (!_enableThrottle) {
        emit valueChanged(currentValue);
        emit textChanged(currentText);
        return;
    }
    //  执行定时器节流
    if (_throttleTimer == nullptr) {
        // 第一次触发必须更新
        emit valueChanged(currentValue);
        emit textChanged(currentText);
        _throttleTimer = new QTimer();
    }
    else if (!_throttleTimer->isActive()) {
        // 上一次定时器结束才能进来
        if (_throttleTimer != nullptr) {
            delete _throttleTimer;
        }
        _throttleTimer = new QTimer();
        _throttleTimer->setSingleShot(true); // 只执行一次
        connect(_throttleTimer, &QTimer::timeout, this, [=]() {
            emit valueChanged(currentValue);
            emit textChanged(currentText);
            delete _throttleTimer;
            _throttleTimer = nullptr;
        });
        _throttleTimer->start(delay);
    }
}
void EprLineEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Up) {
        handleKeyUpOrDown(true);
    }
    else if (event->key() == Qt::Key_Down) {
        handleKeyUpOrDown(false);
    }
    else if (event->key() == Qt::Key_Return) {
        whileEnter();
    }
    else if (event->key() == Qt::Key_Enter) {
        whileEnter();
    }
    else {
        QWidget::keyPressEvent(event);
    }
}
void EprLineEdit::handleKeyUpOrDown(bool up)
{
    const QString value = this->text();

    int pos     = ui->input->cursorPosition();
    int len     = value.length();
    auto number = value.toDouble();
    // 点的位置应该和整数部分长度一致
    const QString& text = lineEdit()->text();
    int dotPos          = text.indexOf('.');
    // int dotPos = QString::number(value.toInt()).length();
    int count = (dotPos < 0 ? text.length() : dotPos) - pos;

    if (count != -1 && pos != 0) {
        count = count < 0 ? count + 1 : count;

        if (up) {
            number += pow(10, count);
        }
        else {
            number -= pow(10, count);
        }
        const QString& newText = QString::number(number, 'f', _decimals);
        // this->setText(newText);
        setValue(number, true);
        int index = pos - 1 + newText.length() - len;
        index     = qMax(index, 0); // 不能小于0
        ui->input->setSelection(index, 1);
        onTextChanged(newText);
    }
}

void EprLineEdit::whileEnter()
{
    // this->setValue(lineEdit->text().toDouble());
    this->clearFocus();
}