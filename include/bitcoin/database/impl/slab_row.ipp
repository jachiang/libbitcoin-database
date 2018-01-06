/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_DATABASE_SLAB_ROW_IPP
#define LIBBITCOIN_DATABASE_SLAB_ROW_IPP

#include <cstddef>
#include <cstdint>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/database/define.hpp>
#include <bitcoin/database/memory/memory.hpp>
#include <bitcoin/database/primitives/slab_manager.hpp>

namespace libbitcoin {
namespace database {

template <typename KeyType, typename LinkType>
slab_row<KeyType, LinkType>::slab_row(slab_manager<LinkType>& manager)
  ////: slab_row(manager_, not_found) <= VC++ compiler bug?
  : manager_(manager), position_(not_found)
{
}

template <typename KeyType, typename LinkType>
slab_row<KeyType, LinkType>::slab_row(slab_manager<LinkType>& manager,
    LinkType position)
  : manager_(manager), position_(position)
{
}

template <typename KeyType, typename LinkType>
LinkType slab_row<KeyType, LinkType>::create(const KeyType& key,
    write_function write, size_t value_size)
{
    BITCOIN_ASSERT(position_ == not_found);

    // Create new (unlinked) slab and populate its key and data.
    //   [ KeyType  ] <==
    //   [ next:8   ]
    //   [ value... ] <==
    const size_t slab_size = prefix_size + value_size;
    position_ = manager_.new_slab(slab_size);

    const auto memory = raw_data(key_start);
    const auto key_data = memory->buffer();
    auto serial = make_unsafe_serializer(key_data);
    serial.write_forward(key);
    serial.skip(link_size);
    serial.write_delegated(write);

    return position_;
}

template <typename KeyType, typename LinkType>
void slab_row<KeyType, LinkType>::link(LinkType next)
{
    // Populate next pointer value.
    //   [ KeyType  ]
    //   [ next:8   ] <==
    //   [ value... ]

    // Write next pointer after the key.
    const auto memory = raw_data(key_size);
    const auto next_data = memory->buffer();
    auto serial = make_unsafe_serializer(next_data);

    //*************************************************************************
    serial.template write_little_endian<LinkType>(next);
    //*************************************************************************
}

template <typename KeyType, typename LinkType>
bool slab_row<KeyType, LinkType>::compare(const KeyType& key) const
{
    const auto memory = raw_data(key_start);
    return std::equal(key.begin(), key.end(), memory->buffer());
}

template <typename KeyType, typename LinkType>
memory_ptr slab_row<KeyType, LinkType>::data() const
{
    // Get value pointer.
    //   [ KeyType  ]
    //   [ next:8   ]
    //   [ value... ] ==>

    // Value data is at the end.
    return raw_data(prefix_size);
}

template <typename KeyType, typename LinkType>
LinkType slab_row<KeyType, LinkType>::offset() const
{
    // Value data is at the end.
    return position_ + prefix_size;
}

template <typename KeyType, typename LinkType>
LinkType slab_row<KeyType, LinkType>::next_position() const
{
    const auto memory = raw_data(key_size);
    const auto next_address = memory->buffer();

    //*************************************************************************
    return from_little_endian_unsafe<LinkType>(next_address);
    //*************************************************************************
}

template <typename KeyType, typename LinkType>
void slab_row<KeyType, LinkType>::write_next_position(LinkType next)
{
    const auto memory = raw_data(key_size);
    auto serial = make_unsafe_serializer(memory->buffer());

    //*************************************************************************
    serial.template write_little_endian<LinkType>(next);
    //*************************************************************************
}

template <typename KeyType, typename LinkType>
memory_ptr slab_row<KeyType, LinkType>::raw_data(size_t bytes) const
{
    auto memory = manager_.get(position_);
    memory->increment(bytes);
    return memory;
}

} // namespace database
} // namespace libbitcoin

#endif
