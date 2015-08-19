#ifndef msg_cache_adl_h_adata_header_define
#define msg_cache_adl_h_adata_header_define

#include <adata.hpp>

namespace p {
  struct xs2ds_entity_req
  {
    uint64_t req_guid;
    ::std::vector< uint8_t > data;
    xs2ds_entity_req()
    :    req_guid(0ULL)
    {}
  };

  struct ds2xs_entity_ack
  {
    uint8_t result;
    uint64_t req_guid;
    ::std::vector< uint8_t > data;
    ds2xs_entity_ack()
    :    result(0),
    req_guid(0ULL)
    {}
  };

}

namespace adata
{
  template<typename stream_ty>
  ADATA_INLINE void read( stream_ty& stream, ::p::xs2ds_entity_req& value)
  {
    ::std::size_t offset = stream.read_length();
    uint64_t tag = 0;
    read(stream,tag);
    if(stream.error()){return;}
    int32_t len_tag = 0;
    read(stream,len_tag);
    if(stream.error()){return;}

    if(tag&1ULL)    {read(stream,value.req_guid);{if(stream.error()){stream.trace_error("req_guid",-1);return;}}}
    if(tag&2ULL)
    {
      uint32_t len = check_read_size(stream);
      {if(stream.error()){stream.trace_error("data",-1);return;}}
      value.data.resize(len);
      for (std::size_t i = 0 ; i < len ; ++i)
      {
        {read(stream,value.data[i]);}
        {if(stream.error()){stream.trace_error("data",(int32_t)i);return;}}
      }
    }
    if(len_tag >= 0)
    {
      ::std::size_t read_len = stream.read_length() - offset;
      ::std::size_t len = (::std::size_t)len_tag;
      if(len > read_len) stream.skip_read(len - read_len);
    }
  }

  template <typename stream_ty>
  ADATA_INLINE void skip_read(stream_ty& stream, ::p::xs2ds_entity_req* value)
  {
    (value);
    skip_read_compatible(stream);
  }

  ADATA_INLINE int32_t size_of(const ::p::xs2ds_entity_req& value)
  {
    int32_t size = 0;
    uint64_t tag = 1ULL;
    if(!value.data.empty()){tag|=2ULL;}
    {
      size += size_of(value.req_guid);
    }
    if(tag&2ULL)
    {
      int32_t len = (int32_t)(value.data).size();
      size += size_of(len);
      for (::std::vector< uint8_t >::const_iterator i = value.data.begin() ; i != value.data.end() ; ++i)
      {
        size += size_of(*i);
      }
    }
    size += size_of(tag);
    size += size_of(size + size_of(size));
    return size;
  }

  template<typename stream_ty>
  ADATA_INLINE void write(stream_ty& stream , const ::p::xs2ds_entity_req&value)
  {
    uint64_t tag = 1ULL;
    if(!value.data.empty()){tag|=2ULL;}
    write(stream,tag);
    if(stream.error()){return;}
    write(stream,size_of(value));
    if(stream.error()){return;}
    {write(stream,value.req_guid);{if(stream.error()){stream.trace_error("req_guid",-1);return;}}}
    if(tag&2ULL)
    {
      uint32_t len = (uint32_t)(value.data).size();
      write(stream,len);
      int32_t count = 0;
      for (::std::vector< uint8_t >::const_iterator i = value.data.begin() ; i != value.data.end() ; ++i, ++count)
      {
        {write(stream,*i);}
        {if(stream.error()){stream.trace_error("data",count);return;}}
      }
    }
    return;
  }

  template<typename stream_ty>
  ADATA_INLINE void read( stream_ty& stream, ::p::ds2xs_entity_ack& value)
  {
    ::std::size_t offset = stream.read_length();
    uint64_t tag = 0;
    read(stream,tag);
    if(stream.error()){return;}
    int32_t len_tag = 0;
    read(stream,len_tag);
    if(stream.error()){return;}

    if(tag&1ULL)    {read(stream,value.req_guid);{if(stream.error()){stream.trace_error("req_guid",-1);return;}}}
    if(tag&2ULL)    {read(stream,value.result);{if(stream.error()){stream.trace_error("result",-1);return;}}}
    if(tag&4ULL)
    {
      uint32_t len = check_read_size(stream);
      {if(stream.error()){stream.trace_error("data",-1);return;}}
      value.data.resize(len);
      for (std::size_t i = 0 ; i < len ; ++i)
      {
        {read(stream,value.data[i]);}
        {if(stream.error()){stream.trace_error("data",(int32_t)i);return;}}
      }
    }
    if(len_tag >= 0)
    {
      ::std::size_t read_len = stream.read_length() - offset;
      ::std::size_t len = (::std::size_t)len_tag;
      if(len > read_len) stream.skip_read(len - read_len);
    }
  }

  template <typename stream_ty>
  ADATA_INLINE void skip_read(stream_ty& stream, ::p::ds2xs_entity_ack* value)
  {
    (value);
    skip_read_compatible(stream);
  }

  ADATA_INLINE int32_t size_of(const ::p::ds2xs_entity_ack& value)
  {
    int32_t size = 0;
    uint64_t tag = 3ULL;
    if(!value.data.empty()){tag|=4ULL;}
    {
      size += size_of(value.req_guid);
    }
    {
      size += size_of(value.result);
    }
    if(tag&4ULL)
    {
      int32_t len = (int32_t)(value.data).size();
      size += size_of(len);
      for (::std::vector< uint8_t >::const_iterator i = value.data.begin() ; i != value.data.end() ; ++i)
      {
        size += size_of(*i);
      }
    }
    size += size_of(tag);
    size += size_of(size + size_of(size));
    return size;
  }

  template<typename stream_ty>
  ADATA_INLINE void write(stream_ty& stream , const ::p::ds2xs_entity_ack&value)
  {
    uint64_t tag = 3ULL;
    if(!value.data.empty()){tag|=4ULL;}
    write(stream,tag);
    if(stream.error()){return;}
    write(stream,size_of(value));
    if(stream.error()){return;}
    {write(stream,value.req_guid);{if(stream.error()){stream.trace_error("req_guid",-1);return;}}}
    {write(stream,value.result);{if(stream.error()){stream.trace_error("result",-1);return;}}}
    if(tag&4ULL)
    {
      uint32_t len = (uint32_t)(value.data).size();
      write(stream,len);
      int32_t count = 0;
      for (::std::vector< uint8_t >::const_iterator i = value.data.begin() ; i != value.data.end() ; ++i, ++count)
      {
        {write(stream,*i);}
        {if(stream.error()){stream.trace_error("data",count);return;}}
      }
    }
    return;
  }

}

#endif
