#include "wasm-memory/address_chunk.h"

namespace memory {
void AddressChunk::clear() {
  _len = 0;

  for (size_t i = 0; i < ADDRESSCHUNK_LEN; ++i) {
    _addr[i].clear();
  }

  _next = reinterpret_cast<AddressChunk*>(NULL_PTR);
}

size_t AddressChunk::find(void *ptr) {
  if (_len == 0) {
    return 0;
  }

  size_t
    lowIdx = 0,
           highIdx = _len - 1,
           idx;

  void
    *currPtr;

  while (lowIdx <= highIdx) {
    idx = lowIdx + ((highIdx - lowIdx) >> 1);
    currPtr = _addr[idx]._ptr;

    if (currPtr == ptr) {
      break;
    } else {
      if (currPtr < ptr) {
        lowIdx = idx + 1;

        if (lowIdx == _len) {
          idx = lowIdx;
          break;
        }
      } else {
        if (idx == 0) {
          break;
        }

        highIdx = idx - 1;
      }
    }
  }

  return idx;
}

bool AddressChunk::insert(size_t idx, Address &addr) {
  if (idx > _len || idx >= ADDRESSCHUNK_LEN) {
    return false;
  }

  if (_len == ADDRESSCHUNK_LEN) {
    if (_next == NULL_PTR ||
        !_next->insert(0, _addr[_len - 1])) {
      return false;
    }
  } else {
    ++_len;
  }

  for (size_t i = _len - 1; i > idx; --i) {
    _addr[i] = _addr[i - 1];
  }

  _addr[idx] = addr;

  return true;
}

bool AddressChunk::insert(size_t idx, void *ptr, size_t len) {
  if (idx > _len || idx >= ADDRESSCHUNK_LEN) {
    return false;
  }

  if (_len == ADDRESSCHUNK_LEN) {
    if (_next == NULL_PTR ||
        !_next->insert(0, _addr[_len - 1])) {
      return false;
    }
  } else {
    ++_len;
  }

  for (size_t i = _len - 1; i > idx; --i) {
    _addr[i] = _addr[i - 1];
  }

  _addr[idx]._ptr = ptr;
  _addr[idx]._len = len;

  return true;
}

bool AddressChunk::remove(size_t idx) {
  if (idx >= _len) {
    return false;
  }

  size_t len = _len - 1;

  for (size_t i = idx; i < len; ++i) {
    _addr[i] = _addr[i + 1];
  }

  if (_len == ADDRESSCHUNK_LEN) {
    if (_next != NULL_PTR && _next->_len > 0) {
      _addr[_len - 1] = _next->_addr[0];

      if (!_next->remove(0)) {
        return false;
      }
    } else {
      _addr[_len - 1].clear();

      --_len;
    }
  } else {
    _addr[_len - 1].clear();

    --_len;
  }

  return true;
}

#ifndef CATENA_PLATFORM_WASM
  std::ostream &operator<<(std::ostream &stream, const AddressChunk &chunk) {
    stream << "length   " << std::hex << std::setw(2) << std::setfill('0') <<
      chunk._len << std::endl;

    for (size_t i = 0; i < chunk._len; ++i) {
      stream << std::hex << std::setw(2) << std::setfill('0') << i << "       "
        << chunk._addr[i] << std::endl;
    }

    return stream << "-> chunk " << chunk._next << std::endl;
  }
#endif
}  // namespace memory
