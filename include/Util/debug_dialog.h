#ifndef ZZ_DEBUG_DIALOG_TO_SHOW_DEBUG_INFO_2008071212__H___
#define ZZ_DEBUG_DIALOG_TO_SHOW_DEBUG_INFO_2008071212__H___


/**
 * @author Thinapple (turingmc@gmail.com)
 */
#include <QTextEdit>
#include <QPointer>
#include <string>
#include <sstream>

namespace Swift
{
;

class DebugDialog
{
private:
	DebugDialog(const DebugDialog &);
	const DebugDialog & operator = (const DebugDialog &);
public:
	DebugDialog();
	~DebugDialog();

	static DebugDialog & globalInstance();
public:
	void show();
	void hide();
	void clear();
	template <typename T>
	DebugDialog & operator << (const T & elem)
	{
		std::stringstream ss;
		ss << elem;
		append(ss.str());

		return *this;
	}
	template <typename T>
	DebugDialog & append(const T & elem)
	{
		std::stringstream ss;
		ss << elem;
		append(ss.str());

		return *this;
	}
	DebugDialog & append(const std::string & text);
	DebugDialog & insertHtml(const std::string & text);
private:
	QPointer<QTextEdit>	m_out;
};

/*
template <typename T>
DebugDialog & operator << (DebugDialog & dialog, const T & elem)
{
	return ( dialog  << elem );
}
*/

} // namespace Swift

#endif

