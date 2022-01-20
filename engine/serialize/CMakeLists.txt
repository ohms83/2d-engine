list(APPEND ENGINE_HEADER
    engine/serialize/IniParser.hpp
    engine/serialize/JsonSerializer.hpp
    engine/serialize/MsgpackSerializer.hpp
    engine/serialize/Serialize.hpp
)

list(APPEND ENGINE_SOURCE
    engine/serialize/IniParser.cpp
    engine/serialize/JsonSerializer.cpp
    engine/serialize/MsgpackSerializer.cpp
    engine/serialize/Serialize.cpp
)