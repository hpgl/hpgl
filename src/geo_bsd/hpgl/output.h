#ifndef __OUTPUT_H__AA0B5121_31E3_4DFA_9EEB_C1EACF2AC47B
#define __OUTPUT_H__AA0B5121_31E3_4DFA_9EEB_C1EACF2AC47B

namespace hpgl
{
	void write(const char * str);
	void write(const std::string & str);
	void write(const boost::format & str);	

	void update_progress(const char * stage, int percentage);
}

#define LOGWARNING(msg) hpgl::write(boost::format("%1%:%2% %3% Warning: %4%") % __FILE__ % __LINE__ % __FUNCTION__ % msg);

#endif //__OUTPUT_H__AA0B5121_31E3_4DFA_9EEB_C1EACF2AC47B
