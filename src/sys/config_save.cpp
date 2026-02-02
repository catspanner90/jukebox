#include "sys/config_save.h"

#include "ibn_sram_rw.h"

#include <type_traits>

namespace jb::sys
{

namespace
{

constexpr bn::string_view SAVE_MAGIC = "CSPJB";

constexpr unsigned SAVE_LOCATION_0 = bn::sram::size() - 512;
constexpr unsigned SAVE_LOCATION_1 = bn::sram::size() - 256;

constexpr std::uint32_t FOOTER = 0x5A7EF001; // SAVE FOOT

} // namespace

config_save::config_save()
{
    reset();
}

void config_save::reset()
{
}

bool config_save::load()
{
    ibn::sram_rw rw(SAVE_MAGIC, SAVE_LOCATION_0, SAVE_LOCATION_1);

    const bool loaded = rw.read(*this);

    // If read fails, it might be halfway-loaded (inconsistent state),
    // so we reset again.
    if (!loaded)
        reset();

    return loaded;
}

void config_save::save()
{
    ibn::sram_rw rw(SAVE_MAGIC, SAVE_LOCATION_0, SAVE_LOCATION_1);
    rw.write(*this);
}

void config_save::measure(ibn::bit_stream_measurer& measurer) const
{
    measurer.write(FOOTER); // footer: 32 bits
}

void config_save::write(ibn::bit_stream_writer& writer) const
{
    writer.write(FOOTER); // footer: 32 bits
}

void config_save::read(ibn::bit_stream_reader& reader)
{
    std::uint32_t footer = 0;

    reader.read(footer); // footer: 32 bits

    if (footer != FOOTER)
        reader.set_fail();
}

} // namespace jb::sys
