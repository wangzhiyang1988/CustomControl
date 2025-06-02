/********************************************************************************
** @brief  对整形和浮点数据有效位判断处理
** @date   2025.06.02
** @author wangzy
********************************************************************************/
#ifndef EPR100LINEEDITCHECK
#define EPR100LINEEDITCHECK
#include <QValidator>

class EprValidator : public QValidator {

};
class Epr100DoubleValidator : public QDoubleValidator
{
	Q_OBJECT
public:
	explicit Epr100DoubleValidator(double bottom, double top, int decimals, QObject *parent = 0)
		:QDoubleValidator(bottom, top, decimals, parent) {
		m_decimals = decimals;
	}
	~Epr100DoubleValidator() {}

	virtual QValidator::State validate(QString &input, int &pos) const;
	virtual void fixup(QString &input)const;
private:
	int m_decimals = 0;
};


class Epr100IntValidator:public QIntValidator
{
	Q_OBJECT
public:
	explicit Epr100IntValidator(QObject *parent) :QIntValidator(parent) {}
	Epr100IntValidator(int bottom, int top, QObject *parent = 0) 
		:QIntValidator(bottom, top, parent) {}
	~Epr100IntValidator() {}

	//virtual QValidator::State validate(QString &input, int &pos) const;
	virtual void fixup(QString &input)const;
};

#endif //EPR100LINEEDITCHECK

