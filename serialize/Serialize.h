//
// Created by TempUser on 2022/12/4.
//

#ifndef CPPSERIALIZE_SERIALIZE_H
#define CPPSERIALIZE_SERIALIZE_H

namespace yazi {
    namespace serialize {
        class DataStream;

        class Serializable {
        public:
            virtual void serialize(DataStream &stream) const = 0;
            virtual bool deserialize(DataStream &stream) = 0;
        };

#define SERIALIZE(...)                                  \
    void serialize(DataStream &stream) const {          \
        stream.set_type(DataStream::DataType::CUSTOM);  \
        stream.write_args(__VA_ARGS__);                 \
    }                                                   \
    bool deserialize(DataStream &stream) {              \
        auto type = stream.get_type();                  \
        if (type != DataStream::DataType::CUSTOM) {     \
            return false;                               \
        }                                               \
        stream.read_args(__VA_ARGS__);                  \
        return true;                                    \
    }

    }
}

#endif //CPPSERIALIZE_SERIALIZE_H
