/**
 * @author Thinapple (turingmc@gmail.com)
 */
#include "Util/debug_dialog.h"

namespace Swift
{
;

DebugDialog::DebugDialog()
{
	m_out = new QTextEdit;

	return ;
} // 

DebugDialog::~DebugDialog()
{
	return ;
} //

DebugDialog & DebugDialog::globalInstance()
{
	static DebugDialog dialog;
	return dialog;
} // globalInstance()

void DebugDialog::show()
{
	m_out ->show();
} // show()

void DebugDialog::hide()
{
	m_out ->hide();
} // hide()

void DebugDialog::clear()
{
	m_out ->clear();
} // clear()

DebugDialog & DebugDialog::append(const std::string & text)
{
	std::stringstream result;
	for(int i = 0; i < text.length(); ++i){
		if( '\n' == text[i] ){
			result << "<br>";
		}else{
			result << text[i];
		}
	}
	insertHtml(result.str());
	return *this;
} // append( text )

DebugDialog & DebugDialog::insertHtml(const std::string & text)
{
	m_out ->insertHtml( QString::fromStdString( text ) );

	return *this;
} // insertHtml( text )

} // namespace Swift

