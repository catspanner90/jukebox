#pragma once

#include <cstdint>

namespace ibn
{

class bit_stream_measurer;
class bit_stream_writer;
class bit_stream_reader;

} // namespace ibn

namespace jb::sys
{

class config_save final
{
public:
    config_save();

    config_save(const config_save&) = delete;
    config_save& operator=(const config_save&) = delete;

public:
    void reset();

    bool load();
    void save();

public:
    unsigned tune_index() const;
    void set_tune_index(unsigned index);

public: // `sram_rw` uses these to save/load
    void measure(ibn::bit_stream_measurer& measurer) const;
    void write(ibn::bit_stream_writer& writer) const;
    void read(ibn::bit_stream_reader& reader);

private:
    unsigned _tune_index;
};

} // namespace jb::sys
