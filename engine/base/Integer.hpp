#pragma once

#include <cstdint>

namespace base
{
    /// A non-overflow unsigned integer
    class Uint final
    {
    public:
        Uint() {}

        Uint(const Uint& rhs)
        {
            _value = rhs._value;
        }

        template<typename T>
        Uint(T rhs)
        {
            if (rhs < 0) {
                _value = 0;
            }
            else if (rhs > UINT32_MAX) {
                _value = UINT32_MAX;
            }
            else {
                _value = (uint32_t)rhs;
            }
        }

        template<typename T>
        T operator + (T rhs)
        {
            return _value + rhs;
        }
        Uint operator + (const Uint& rhs)
        {
            return Uint(_value + rhs._value);
        }

        template<typename T>
        T operator - (T rhs)
        {
            return _value - rhs;
        }
        Uint operator - (const Uint& rhs)
        {
            return Uint(_value - rhs._value);
        }

        template<typename T>
        T operator * (T rhs)
        {
            return _value * rhs;
        }
        Uint operator * (const Uint& rhs)
        {
            return Uint(_value * rhs._value);
        }

        template<typename T>
        T operator / (T rhs)
        {
            return _value / rhs;
        }
        Uint operator / (const Uint& rhs)
        {
            return Uint(_value / rhs._value);
        }

        template<typename T>
        Uint& operator += (T rhs)
        {
            int64_t newValue = (int64_t)_value + (int64_t)rhs;
            return (*this) = newValue;
        }
        Uint& operator += (const Uint& rhs)
        {
            int64_t newValue = (int64_t)_value + (int64_t)rhs._value;
            return (*this) = newValue;
        }

        template<typename T>
        Uint& operator -= (T rhs)
        {
            int64_t newValue = (int32_t)_value - (int32_t)rhs;
            return (*this) = newValue;
        }
        Uint& operator -= (const Uint& rhs)
        {
            int64_t newValue = (int64_t)_value - (int64_t)rhs._value;
            return (*this) = newValue;
        }

        template<typename T>
        Uint& operator *= (T rhs)
        {
            T newValue = _value * rhs;
            return (*this) = newValue;
        }
        Uint& operator *= (const Uint& rhs)
        {
            int64_t newValue = (int64_t)_value * (int64_t)rhs._value;
            return (*this) = newValue;
        }

        template<typename T>
        Uint& operator /= (T rhs)
        {
            T newValue = _value / rhs;
            return (*this) = newValue;
        }
        Uint& operator /= (const Uint& rhs)
        {
            int64_t newValue = (int64_t)_value / (int64_t)rhs._value;
            return (*this) = newValue;
        }

        template<typename T>
        bool operator == (T rhs) const
        {
            return _value == (T)rhs;
        }
        bool operator == (const Uint& rhs) const
        {
            return _value == rhs._value;
        }

        template<typename T>
        bool operator != (T rhs) const
        {
            return _value != (T)rhs;
        }
        bool operator != (const Uint& rhs) const
        {
            return _value != rhs._value;
        }

        template<typename T>
        Uint& operator = (T rhs)
        {
            (*this) = Uint(rhs);
            return *this;
        }

        template<typename T>
        operator T () const
        {
            return (T)_value;
        }

        uint32_t get() const
        {
            return _value;
        }

        static const Uint ZERO;
    private:
        uint32_t _value = 0;
    };
}