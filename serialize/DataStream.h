//
// Created by TempUser on 2022/12/3.
//

#ifndef CPPSERIALIZE_DATASTREAM_H
#define CPPSERIALIZE_DATASTREAM_H

#include "vector"
#include "cstring"
#include "cstdint"
#include "string"
#include "iostream"

namespace yazi {
    namespace serialize {
        class DataStream {
        public:
            enum DataType { // 1 = 1Byte
                BOOL = 0,   // 2 = Type(1)+Value(1)
                CHAR,       // 2 = Type(1)+Value(1)
                INT32,      // 5 = Type(1)+Value(4)
                INT64,      // 9 = Type(1)+Value(8)
                FLOAT,      // 5 = Type(1)+Value(4)
                DOUBLE,     // 9 = Type(1)+Value(8)
                STRING,     // var = Type(1)+Len(5)(type+value)+value(var)
                VECTOR,
                LIST,
                MAP,
                SET,
                CUSTOM
            };
        public:
            DataStream() : m_pos(0) {};

            ~DataStream() = default;

            std::size_t size() const;
            void display() const;
            void display(std::ostream &out) const;
            void write(bool value);
            void write(char value);
            void write(int32_t value);
            void write(int64_t value);
            void write(float value);
            void write(double value);
            void write(const char *value);
            void write(const std::string &value);
            bool read(bool &value);
            bool read(char &value);
            bool read(int32_t &value);
            bool read(int64_t &value);
            bool read(float &value);
            bool read(double &value);
            bool read(std::string &value);

            DataStream &operator<<(bool value);
            DataStream &operator<<(char value);
            DataStream &operator<<(int32_t value);
            DataStream &operator<<(int64_t value);
            DataStream &operator<<(float value);
            DataStream &operator<<(double value);
            DataStream &operator<<(const char *value);
            DataStream &operator<<(const std::string &value);
            DataStream &operator>>(bool &value);
            DataStream &operator>>(char &value);
            DataStream &operator>>(int32_t &value);
            DataStream &operator>>(int64_t &value);
            DataStream &operator>>(float &value);
            DataStream &operator>>(double &value);
            DataStream &operator>>(std::string &value);

        private:
            std::vector<char> m_buf;
            int m_pos;

        private:
            // 写入数据
            void write(const char *data, int len);

            void reserve(int len);
        };

        void DataStream::write(const char *data, int len) {
            // 预留buf大小
            reserve(len);
            // 实际扩容
            std::size_t size = m_buf.size();
            m_buf.resize(size + len);
            // 数据拷贝
            std::memcpy(&m_buf[size], data, len);
        }

        void DataStream::reserve(int len) {
            std::size_t size = m_buf.size();
            std::size_t cap = m_buf.capacity();
            if (size + len > cap) {
                // 扩容capacity
                while (size + len > cap) {
                    if (cap == 0) {
                        cap = 1;
                    } else {
                        cap *= 2;
                    }
                }
                m_buf.reserve(cap);
            }
        }

        void DataStream::write(bool value) {
            char type = DataType::BOOL;
            // write type
            write((char *) &type, sizeof(char));
            // write value
            write((char *) &value, sizeof(char));
        }

        void DataStream::write(char value) {
            char type = DataType::CHAR;
            // write type
            write((char *) &type, sizeof(char));
            // write value
            write((char *) &value, sizeof(char));
        }

        void DataStream::write(int32_t value) {
            char type = DataType::INT32;
            // write type
            write((char *) &type, sizeof(char));
            // write value
            write((char *) &value, sizeof(int32_t));
        }

        void DataStream::write(int64_t value) {
            char type = DataType::INT64;
            // write type
            write((char *) &type, sizeof(char));
            // write value
            write((char *) &value, sizeof(int64_t));
        }

        void DataStream::write(float value) {
            char type = DataType::FLOAT;
            // write type
            write((char *) &type, sizeof(char));
            // write value
            write((char *) &value, sizeof(float));
        }

        void DataStream::write(double value) {
            char type = DataType::DOUBLE;
            // write type
            write((char *) &type, sizeof(char));
            // write value
            write((char *) &value, sizeof(double));
        }

        void DataStream::write(const char *value) {
            char type = DataType::STRING;
            // write type
            write((char *) &type, sizeof(char));
            // write len
            auto size = (int32_t) strlen(value);
            write(size);
            // write value
            write(value, size);
        }

        void DataStream::write(const std::string &value) {
            char type = DataType::STRING;
            // write type
            write((char *) &type, sizeof(char));
            // write len
            auto size = (int32_t) value.size();
            write(size);
            // write value
            write(value.c_str(), size);
        }

        void DataStream::display() const {
            display(std::cout);
        }

        void DataStream::display(std::ostream &out) const {
            std::size_t size = m_buf.size();
            out << "data size=" << size << std::endl;
            std::size_t i = 0;
            while (i < size) {
                switch ((DataType) m_buf[i]) {
                    case DataType::BOOL:
                        if ((int) m_buf[++i] == 0) {
                            out << std::boolalpha << false;
                        } else {
                            out << std::boolalpha << true;
                        }
                        ++i;
                        break;
                    case DataType::CHAR:
                        out << m_buf[++i];
                        ++i;
                        break;
                    case DataType::INT32:
                        out << *((int32_t *) (&m_buf[++i]));
                        i += sizeof(int32_t);
                        break;
                    case DataType::INT64:
                        out << *((int64_t *) (&m_buf[++i]));
                        i += sizeof(int64_t);
                        break;
                    case DataType::FLOAT:
                        out << *((float *) (&m_buf[++i]));
                        i += sizeof(float);
                        break;
                    case DataType::DOUBLE:
                        out << *((double *) (&m_buf[++i]));
                        i += sizeof(double);
                        break;
                    case DataType::STRING: {
                        if ((DataType) m_buf[++i] != DataType::INT32) {
                            throw std::logic_error("parse string length error, length type not int32");
                        }
                        int len = *((int32_t *) (&m_buf[++i]));
                        i += sizeof(int32_t);
                        out << std::string(&m_buf[i], len);
                        i += len;
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        std::size_t DataStream::size() const {
            return m_buf.size();
        }

        DataStream &DataStream::operator<<(bool value) {
            write(value);
            return *this;
        }

        DataStream &DataStream::operator<<(char value) {
            write(value);
            return *this;
        }

        DataStream &DataStream::operator<<(int32_t value) {
            write(value);
            return *this;
        }

        DataStream &DataStream::operator<<(int64_t value) {
            write(value);
            return *this;
        }

        DataStream &DataStream::operator<<(float value) {
            write(value);
            return *this;
        }

        DataStream &DataStream::operator<<(double value) {
            write(value);
            return *this;
        }

        DataStream &DataStream::operator<<(const char *value) {
            write(value);
            return *this;
        }

        DataStream &DataStream::operator<<(const std::string &value) {
            write(value);
            return *this;
        }

        bool DataStream::read(bool &value) {
            if ((DataType) m_buf[m_pos] != DataType::BOOL) {
                return false;
            }
            ++m_pos;
            value = m_buf[m_pos];
            ++m_pos;
            return true;
        }

        bool DataStream::read(char &value) {
            if ((DataType) m_buf[m_pos] != DataType::CHAR) {
                return false;
            }
            ++m_pos;
            value = m_buf[m_pos];
            ++m_pos;
            return true;
        }

        bool DataStream::read(int32_t &value) {
            if ((DataType) m_buf[m_pos] != DataType::INT32) {
                return false;
            }
            ++m_pos;
            value = *((int32_t *) &m_buf[m_pos]);
            m_pos += sizeof(int32_t);
            return true;
        }

        bool DataStream::read(int64_t &value) {
            if ((DataType) m_buf[m_pos] != DataType::INT64) {
                return false;
            }
            ++m_pos;
            value = *((int64_t *) &m_buf[m_pos]);
            m_pos += sizeof(int64_t);
            return true;
        }

        bool DataStream::read(float &value) {
            if ((DataType) m_buf[m_pos] != DataType::FLOAT) {
                return false;
            }
            ++m_pos;
            value = *((float *) &m_buf[m_pos]);
            m_pos += sizeof(float);
            return true;
        }

        bool DataStream::read(double &value) {
            if ((DataType) m_buf[m_pos] != DataType::DOUBLE) {
                return false;
            }
            ++m_pos;
            value = *((double *) &m_buf[m_pos]);
            m_pos += sizeof(double);
            return true;
        }

        bool DataStream::read(std::string &value) {
            if ((DataType) m_buf[m_pos] != DataType::STRING) {
                return false;
            }
            ++m_pos;
            int len;
            bool b = read(len);
            if (len < 0) {
                return false;
            }
            value.assign((char *) &m_buf[m_pos], len);
            m_pos += len;
            return true;
        }

        DataStream &DataStream::operator>>(bool &value) {
            read(value);
            return *this;
        }

        DataStream &DataStream::operator>>(char &value) {
            read(value);
            return *this;
        }

        DataStream &DataStream::operator>>(int32_t &value) {
            read(value);
            return *this;
        }

        DataStream &DataStream::operator>>(int64_t &value) {
            read(value);
            return *this;
        }

        DataStream &DataStream::operator>>(float &value) {
            read(value);
            return *this;
        }

        DataStream &DataStream::operator>>(double &value) {
            read(value);
            return *this;
        }

        DataStream &DataStream::operator>>(std::string &value) {
            read(value);
            return *this;
        }
    }
}

#endif //CPPSERIALIZE_DATASTREAM_H