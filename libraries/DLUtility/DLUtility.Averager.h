#ifndef _DL_AVERAGER_H_
#define _DL_AVERAGER_H_

/*
 * Defines and Typedefs
 */

template <typename T>
class Averager
{
	public:
		Averager(uint16_t size);
		void reset(T * value);
		uint16_t size(void);
		float getFloatAverage(void);
		T getAverage(void);
		void newData(T NewData);
		uint16_t N(void);
		bool full(void);
		
		#ifdef TEST
		void fillFromArray(T * array, uint16_t size);
		#endif

	private:
		T * m_data;
		uint16_t m_write;
		uint16_t m_maxIndex;
		bool m_full;
};

#endif
