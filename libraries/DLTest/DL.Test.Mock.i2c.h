#ifndef _DL_TEST_MOCK_I2C_H_
#define _DL_TEST_MOCK_I2C_H_

class I2CMock
{
	public:

		void begin(void);
		void beginTransmission(uint8_t);
		void endTransmission(void);

		void requestFrom(uint8_t, uint8_t);
		uint8_t receive(void);
		void send(uint8_t);

		void setSentBuffer(uint8_t * pBuf) {m_sentBuffer = pBuf; m_sent = 0;}
		void setRecvdBuffer(uint8_t * pBuf)  {m_recvdBuffer = pBuf; m_recvd = 0;}

		uint8_t sentCount(void) { return m_sent; }
		uint8_t recvdCount(void) { return m_recvd; }

	private:
		uint8_t * m_sentBuffer;
		uint8_t * m_recvdBuffer;
		uint8_t m_addr;
		uint8_t m_sent;
		uint8_t m_recvd;
		
};

#endif
