/*
 * 编辑：
     wangzy 2025/06/02
*/
#ifndef MYDBINPUT_H
#define MYDBINPUT_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QDebug>
#include <QLineEdit>
#include "./Epr100Validator/Epr100Validator.h"
// EprLineEdit
namespace Ui {
class EprLineEditClass;

}
class  EprLineEdit : public QWidget
{
	Q_OBJECT
public:
	explicit EprLineEdit(QWidget* parent = nullptr);
	explicit EprLineEdit(const QString& text, QWidget* parent);
	~EprLineEdit();
signals:
	// 通知值更新的信号
	void editingFinished();
	void textChanged(const QString& text);
	void valueChanged(double value);
public:
	/**
	 * @brief 设置当前显示值
	 * @param value 
	 * @param notify 触发valueChange信号
	*/
	void setValue(double value, bool notify = false);
	double value();
	/**
	 * @brief  设置当前显示值
	 * @param text 
	 * @param notify 触发valueChange信号
	 * @return 
	*/
	bool setText(const QString& text, bool notify = true);
	QString text() const;
	/**
	 * @brief 设置验证器
	 * @param validator 
	*/
	void setValidator(const QValidator* validator);
	//
	void setReadOnly(bool);
	bool isReadOnly();

	void setAlignment(Qt::Alignment flag);
	// 最大长度
	void setMaxLength(int);

	void setPlaceholderText(const QString& text);

	/**
	 * @brief setStep   设置按键步进
	 * @param step
	 */
	void setStep(double step);

	/**
	 * @brief SetThrottle   控制是否开启节流
	 * @param _throttle     true-节流  false-关闭节流
	 */
	void setThrottle(bool enable);

	/**
	 * @brief SetTimerDelay     设置定时器节流时间
	 * @param _delay    单位ms
	 */
	void setTimerDelay(int _delay = 100);

	/**
	 * @brief SetEnable         设置组件启用状态
	 * @param _enable       默认true
	 */
	void setEnable(bool _enable = true);

	/**
	 * @brief SetDecimals       设置精度
	 * @param _decimals         默认保留3位小数
	 */
	void setDecimals(int _decimals = 0);
	int decimals() const;
	/**
	 * @brief SetBtnNone        设置按钮不显示
	 */
	void hideButton() const;

	void clearFocus();
	/*
	 * @brief 获取最大值
	 */
    double getMaxValue() const;
    /*
     * @brief 获取最小值
     */
    double getMinValue() const;

protected:
	bool eventFilter(QObject* obj, QEvent* e) override;
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	void on_btnSub_clicked();

	void on_btnAdd_clicked();

	void onTextChanged(const QString& text);

private:
	void setMinMaxRange(double min, double max);
	void handleKeyUpOrDown(bool up = true);
	void whileEnter();

public:
	QLineEdit* lineEdit();

private:
	Ui::EprLineEditClass* ui;
	QTimer* _throttleTimer = nullptr;      // 用于节流的定时器

	bool _enableThrottle = true;    // true-节流输出；false-不节流
	bool _notify = true;          // 是否发送valueChanged信号

	// 量程
	double _maxValue = 4095.0;
	double _minValue = 0.0;
	double _step = 0.5;      //按键 步进

	// 节流时间 ms
	int delay = 200;

	double _lastPosX = 0;    // 缓存上一次的move X， 防止两次move差值过大
	// 精度
	double _decimals = 3;
};

#endif // MYDBINPUT_H
