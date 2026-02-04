#pragma once

#include <bn_span.h>
#include <bn_string_view.h>

namespace bn
{
class dmg_music_item;
class direct_bitmap_item;
} // namespace bn

namespace jb
{

class tune_info final
{
public:
    static auto tunes_list() -> bn::span<const tune_info>;
    static auto tunes_names_list() -> bn::span<const bn::string_view>;

public:
    constexpr tune_info(const bn::dmg_music_item& tune, bool loop, const bn::direct_bitmap_item* thumbnail,
                        const bn::string_view& tune_name, const bn::string_view& composer_name,
                        const bn::string_view& remixer_name, const bn::string_view& description)
        : _tune(tune), _loop(loop), _thumbnail(thumbnail), _tune_name(tune_name), _composer_name(composer_name),
          _remixer_name(remixer_name), _description(description)
    {
    }

private:
    const bn::dmg_music_item& _tune;
    bool _loop;
    const bn::direct_bitmap_item* _thumbnail;
    bn::string_view _tune_name;
    bn::string_view _composer_name;
    bn::string_view _remixer_name;
    bn::string_view _description;

public:
    constexpr auto tune() const -> decltype(_tune)
    {
        return _tune;
    }

    constexpr bool loop() const
    {
        return _loop;
    }

    constexpr auto thumbnail() const -> decltype(_thumbnail)
    {
        return _thumbnail;
    }

    constexpr auto tune_name() const -> decltype((_tune_name))
    {
        return _tune_name;
    }

    constexpr auto composer_name() const -> decltype((_composer_name))
    {
        return _composer_name;
    }

    constexpr auto remixer_name() const -> decltype((_remixer_name))
    {
        return _remixer_name;
    }

    constexpr auto description() const -> decltype((_description))
    {
        return _description;
    }
};

} // namespace jb
