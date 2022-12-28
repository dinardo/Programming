#ifndef PACKERUNPACKER_H
#define PACKERUNPACKER_H

#include <assert.h>
#include <tuple>
#include <array>


namespace bitWise
{
  // ###############################
  // # Sum template parameter pack #
  // ###############################
  template<size_t size, size_t... sizes>
  struct sumSizes
  {
    static constexpr size_t sum = size + sumSizes<sizes...>::sum;
  };

  template<size_t size>
  struct sumSizes<size> // <size> here is needed in order to allow the compiler to distinguish between the two struct types
  {
    static constexpr size_t sum = size;
  };


  // ############################################################
  // # Generate sequence of values to be used as parameter pack #
  // ############################################################
  template<size_t...> struct sequenceType {};

  template <size_t N, size_t value, size_t... Is>
  struct genSequenceType : genSequenceType<N - 1, value, value, Is...> {};

  template <size_t value, size_t... Is>
  struct genSequenceType<1u, value, Is...>
  {
    using type = sequenceType<value, Is...>;
  };

  template <size_t N, size_t value>
  using genSequence = typename genSequenceType<N,value>::type;


  // ################
  // # General case #
  // ################
  template<size_t size, size_t... sizes>
  struct bitPacker
  {
    template<size_t offset, typename T>
    static auto unpack (const T& arg)
    {
      // return std::tuple_cat(std::make_tuple((arg >> (offset-size)) & ((1 << size) - 1)), bitPacker<sizes...>::template unpack<offset-size>(arg)); // @TMP@
      return std::tuple_cat(std::make_tuple((arg >> (offset-size)) & static_cast<T>((1 << size) - 1)), bitPacker<sizes...>::template unpack<offset-size>(arg));
    }

    template<size_t offset, typename T, typename... Ts>
    static uint64_t pack (const T& arg, const Ts&... args)
    {
      return (arg & ((1 << size) - 1)) << (offset-size) | bitPacker<sizes...>::template pack<offset-size>(args...);
    }
  };


  // ##############
  // # Last field #
  // ##############
  template<size_t size>
  struct bitPacker<size>  // <size> here is needed in order to allow the compiler to distinguish between the two struct types
  {
    template<size_t offset, typename T>
    static auto unpack (const T& arg)
    {
      // return std::make_tuple((arg >> (offset-size)) & ((1 << size) - 1)); // @TMP@
      return std::make_tuple((arg >> (offset-size)) & static_cast<T>((1 << size) - 1));
    }

    template<size_t offset, typename T>
    static auto pack (const T& arg)
    {
      return (arg & ((1 << size) - 1)) << (offset-size);
    }
  };


  // ###############################
  // # Upacks a value into a tuple #
  // ###############################
  template<size_t... sizes, typename T>
  auto unpack(const T& arg)
  {
    return bitPacker<sizes...>::template unpack<sumSizes<sizes...>::sum>(arg);
    // The sintax with bitPacker<sizes...>::template is needed because it tells to the compiler that unpack is a template
    // In generate template functions are made only when they are called, not before. So in this case the compiler does
    // not know what's the nautre of unpack
  }


  // ################################################################################
  // # Same as unpack but the number of bits is specified once and repeated N times #
  // ################################################################################
  template<typename T, size_t... Is>
  auto unpackSequence(const T& arg, sequenceType<Is...>)
  {
    return bitPacker<Is...>::template unpack<sumSizes<Is...>::sum>(arg);
  }

  template<size_t N, size_t size, typename T>
  auto unpackSequence(const T& arg)
  {
    return unpackSequence(arg,genSequence<N,size>());
  }


  // #############################################
  // # Packs multiple fields into a single value #
  // #############################################
  template<size_t... sizes, typename... Ts>
  auto pack(const Ts&... args)
  {
    return bitPacker<sizes...>::template pack<sumSizes<sizes...>::sum>(args...);
  }


  // ##################
  // # Revert a tuple #
  // ##################
  template<typename T>
  auto revertTupleUnpack(const T& arg)
  {
    return std::make_tuple(arg);
  }

  template<typename T, typename... Ts>
  auto revertTupleUnpack(const T& arg, const Ts&... args)
  {
    return std::tuple_cat(revertTupleUnpack(args...), std::make_tuple(arg));
  }

  template<typename T, size_t... Is>
  auto revertTupleUnpack(const T& arg, std::index_sequence<Is...>)
  // std::index_sequence<Is...> is called "temporary" and it's needed only to get the parameter pack Is...
  {
    return revertTupleUnpack(std::get<Is>(arg)...);
    // The ... can be interpreted as: "unpack the parameter pack and put a comma in between"

    // The same thing can be done by simply doing:
    // return std::make_tuple(std::get<sizeof...(Is) - 1 - Is>(arg)...);
  }

  template<typename T>
  auto revertTuple(const T& arg)
  {
    return revertTupleUnpack(arg, std::make_index_sequence<std::tuple_size<T>::value>());
    // The (), or {}, in std::make_index_sequence<std::tuple_size<T>::value>() allow to instantiate the object
  }


  // ###########################
  // # Unpack tuple into array #
  // ###########################
  template<typename T>
  auto tupleUnpacker(size_t indx, const T& arg)
  {
    assert (indx == 0 && "[bitWise::tupleUnpacker]\tOne argument but index not zero");
    return arg;
  }

  template<typename T, typename... Ts>
  auto tupleUnpacker(size_t indx, const T& arg, const Ts&... args)
  {
    if (indx == 0) return arg;
    return tupleUnpacker(--indx, args...);
  }

  template<typename Arr, typename Tup, size_t... Is>
  auto unpackTuple2Array(const Tup& arg, std::index_sequence<Is...>)
  {
    std::array<Arr, sizeof...(Is)> arr;
    for (size_t i = 0; i < arr.size(); i++)
      arr[i] = static_cast<Arr>(tupleUnpacker(i, std::get<Is>(arg)...));
    return arr;
  }

  template<typename Arr, typename Tup>
  auto unpackTuple2Array(const Tup& arg)
  {
    return unpackTuple2Array<Arr>(arg, std::make_index_sequence<std::tuple_size<Tup>::value>());
  }


  // #################################
  // # Unpack iterable into iterable #
  // #################################
  template<size_t size, typename I, typename O>
  void unpackIterable2Iterable(I inFirst, I inLast, O outFirst)
  {
    constexpr auto inTypeSize = 8 * sizeof(typename std::iterator_traits<I>::value_type);

    static_assert(inTypeSize > size, "[bitWise::unpackIterable2Iterable] The size of the input range's value type is too small");

    size_t excess = 0;
    while(inFirst != inLast)
      {
        // ############
        // # Standard #
        // ############
        auto mask = (1 << size) - 1;
        for(auto i = 0u; i < (inTypeSize - excess) / size; i++)
          {
            *outFirst = (*inFirst >> ((size * i) + excess)) & mask;
            outFirst++;
          }

        auto remainder = (inTypeSize - excess) % size;
        if(remainder != 0)
          {
            // #############
            // # Remainder #
            // #############
            mask = (1 << remainder) - 1;
            *outFirst = (*inFirst >> (inTypeSize - remainder)) & mask;
            inFirst++;

            // ##########
            // # Excess #
            // ##########
            excess = size - remainder;
            mask = (1 << excess) - 1;
            *outFirst |= (*inFirst & mask) << remainder;
            outFirst++;
          }
        else
          {
            excess = 0;
            inFirst++;
          }
      }
  }

}

#endif
