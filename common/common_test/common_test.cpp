#include "pch.h"
#include "CppUnitTest.h"
extern "C"
{
#include "buffer.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace commontest {
TEST_CLASS (TestBuffer)
{
public:
    TEST_METHOD (Init)
    {
        buffer_t * p_buffer = NULL;
        BOOL       result   = buffer_init(&p_buffer, MIN_BUFFER_SIZE);
        Assert::IsTrue(result);
        Assert::IsNotNull(p_buffer->lp_data);
        Assert::AreEqual((ULONG)0, p_buffer->offset);
        Assert::AreEqual((ULONG)16, p_buffer->buffer_len);
        buffer_destroy(&p_buffer);
    }

    TEST_METHOD (Destroy)
    {
        buffer_t * p_buffer = NULL;
        buffer_init(&p_buffer, MIN_BUFFER_SIZE);
        BOOL result = buffer_destroy(&p_buffer);
        Assert::IsTrue(result);
        Assert::IsNull(p_buffer);
    }

    TEST_METHOD (DestroyBad)
    {
        buffer_t * p_buffer = NULL;
        BOOL     result = buffer_destroy(&p_buffer);
        Assert::IsFalse(result);
        Assert::IsNull(p_buffer);
    }
    TEST_METHOD (Append)
    {
        buffer_t * p_buffer = NULL;
        buffer_init(&p_buffer, MIN_BUFFER_SIZE);
        BYTE lp_data[] = { 1, 2, 3, 4, 5 };
        BOOL result = buffer_append(p_buffer, lp_data, sizeof(lp_data));
        Assert::IsTrue(result);
        Assert::AreEqual((ULONG)sizeof(lp_data), p_buffer->offset);

        for (ULONG idx = 0; idx < sizeof(lp_data); ++idx)
        {
            Assert::AreEqual(lp_data[idx], p_buffer->lp_data[idx]);
        }
    }

    TEST_METHOD (AppendResize)
    {
        buffer_t * p_buffer = NULL;
        buffer_init(&p_buffer, MIN_BUFFER_SIZE);
        BYTE lp_data[] = { 1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                           11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
        BOOL result    = buffer_append(p_buffer, lp_data, sizeof(lp_data));
        Assert::IsTrue(result);
        Assert::AreEqual((ULONG)sizeof(lp_data), p_buffer->offset);

        for (ULONG idx = 0; idx < sizeof(lp_data); ++idx)
        {
            Assert::AreEqual(lp_data[idx], p_buffer->lp_data[idx]);
        }
        buffer_destroy(&p_buffer);
    }

    TEST_METHOD (AppendLoop)
    {
        buffer_t * p_buffer = NULL;
        buffer_init(&p_buffer, MIN_BUFFER_SIZE);
        BYTE lp_data[] = { 1, 2, 3, 4, 5 };

        for (ULONG idx = 0; idx < 10; ++idx)
        {
            BOOL result = buffer_append(p_buffer, lp_data, sizeof(lp_data));
            Assert::IsTrue(result);
        }
        Assert::AreEqual((ULONG)(sizeof(lp_data) * 10), p_buffer->offset);
        buffer_destroy(&p_buffer);
    }
    TEST_METHOD (Resize)
    {
        buffer_t * p_buffer = NULL;
        buffer_init(&p_buffer, MIN_BUFFER_SIZE);
        ULONG new_size = MIN_BUFFER_SIZE * 20;
        BOOL  result   = buffer_resize(p_buffer, new_size);
        Assert::IsTrue(result);
        Assert::AreEqual(new_size, p_buffer->buffer_len);
    }

    TEST_METHOD(ResizeSmaller)
    {
        buffer_t * p_buffer = NULL;
        buffer_init(&p_buffer, MIN_BUFFER_SIZE * 10);
        ULONG new_size = MIN_BUFFER_SIZE * 5;
        BOOL  result   = buffer_resize(p_buffer, new_size);
        Assert::IsFalse(result);
        Assert::AreEqual((ULONG)MIN_BUFFER_SIZE * 10, p_buffer->buffer_len);
        buffer_destroy(&p_buffer);
    }

    TEST_METHOD (ResizeInvalidBuffer)
    {
        buffer_t * p_buffer = NULL;
        ULONG      new_size = 10;
        BOOL       result   = buffer_resize(p_buffer, new_size);
        Assert::IsFalse(result);
    }
};
}
