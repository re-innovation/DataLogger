#ifndef _DL_TEST_MOCK_SERIAL_H_
#define _DL_TEST_MOCK_SERIAL_H_

class _Serial
{
    public:
        void begin(int);

        template <typename T>
        void print(T);

        template <typename T>
        void println(T);

        void println(void);
};

#endif

extern _Serial Serial;
