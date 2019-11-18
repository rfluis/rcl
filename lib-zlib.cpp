#include "lib-zlib.hpp"

namespace rcl{

bool is_GZIP(const std::vector<unsigned char>& file){
  if (file[0]==0x1f) if (file[1]==0x8b) if (file[2]==0x08) return true;
  return false;
  }

void ungzip(std::vector<unsigned char>& file){
  std::vector<unsigned char> tmp;
  if (!is_GZIP(file)) return;
  inflate(tmp,file,true);
  std::swap(tmp,file);
  }

int deflate(std::vector<unsigned char>& dst,const std::vector<unsigned char>& src,bool gzip){
  return deflate(dst,src,zlib_cfg(9,0,gzip,0));}
    
int deflate(std::vector<unsigned char>& dst,const std::vector<unsigned char>& src,const zlib_cfg& cfg){
  int ret,flush;
  unsigned int have;
  z_stream strm;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];
  /* allocate deflate state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  rcl::cvstream source(src);
  rcl::vstream dest(dst);
  //ZEXTERN int ZEXPORT deflateInit2 (z_streamp strm, int  level, int  method, int  windowBits, int  memLevel, int  strategy);
  /* z_streamp strm, Puntero a la stream de zlib
   * int  level,      1 a 9. Usar 9 para óptimo compresion
   * int  method,     Z_DEFLATED
   * int  windowBits, de 8 a 15. Sumar 16 para gzip. Poner signo negativo para generar 'deflate' crudo
   * int  memLevel,   1 a 9. Usar 9 para óptimo rendimiento
   * int  strategy ,  
   *                Z_DEFAULT_STRATEGY for normal data, 
   *                Z_FILTERED for data produced by a filter (or predictor), 
   *                Z_HUFFMAN_ONLY to force Huffman encoding only (no string match), or 
   *                Z_RLE to limit match distances to one (run-length encoding).
   *                Z_FIXED (FLAG) prevents the use of dynamic Huffman codes
   * 
   *                Filtered data consists mostly of small values with a somewhat random distribution. 
   *                In this case, the compression algorithm is tuned to compress them better. 
   *                The effect of Z_FILTERED is to force more Huffman coding and less string matching; 
   *                it is somewhat intermediate between Z_DEFAULT_STRATEGY and Z_HUFFMAN_ONLY. 
   *                Z_RLE is designed to be almost as fast as Z_HUFFMAN_ONLY, but give better 
   *                compression for PNG image data. The strategy parameter only affects the 
   *                compression ratio but not the correctness of the compressed output even if 
   *                it is not set appropriately. Z_FIXED prevents the use of dynamic Huffman codes, 
   *                allowing for a simpler decoder for special applications.
   *                
   *                data_type = Z_BINARY Z_TEXT Z_UNKNOWN  */
  if (cfg.getgz())    ret = deflateInit2 (&strm, cfg.getlevel(), Z_DEFLATED, 31,9,cfg.getzs());
  if (cfg.getdf())    ret = deflateInit2 (&strm, cfg.getlevel(), Z_DEFLATED,-15,9,cfg.getzs());
  if (cfg.iszl())     ret = deflateInit2 (&strm, cfg.getlevel(), Z_DEFLATED, 15,9,cfg.getzs());
  if (ret != Z_OK)    return ret;
  /* compress until end of file */
  do {
    strm.avail_in = source.sread(in,CHUNK);
    // if (ferror(source)) { (void)deflateEnd(&strm); return Z_ERRNO; }
    flush = source.eof() ? Z_FINISH : Z_NO_FLUSH;
    strm.next_in = in;
    /* run deflate() on input until output buffer not full, finish
      compression if all of source has been read in */
    do {
      strm.avail_out = CHUNK;
      strm.next_out = out;
      ret = deflate(&strm, flush);  /* no bad return value */
      //assert(ret != Z_STREAM_ERROR); /* state not clobbered */
      have = CHUNK - strm.avail_out;
      //if (fwrite(out, 1, have, dest) != have || ferror(dest)) {(void)deflateEnd(&strm); return Z_ERRNO;}
      dest.swrite(out,have);
    } while (strm.avail_out == 0);
    //assert(strm.avail_in == 0);   /* all input will be used */
    /* done when last data in file processed */
  } while (flush != Z_FINISH);
  //assert(ret == Z_STREAM_END);    /* stream will be complete */
  /* clean up and return */
  (void) deflateEnd(&strm);
  return Z_OK;
}
int inflate(std::vector<unsigned char>& dst,const std::vector<unsigned char>& src,bool gzip){
  int ret;
  unsigned have;
  z_stream strm;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];
  rcl::cvstream source(src);
  rcl::vstream dest(dst);
  /* allocate inflate state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = 0;
  strm.next_in = Z_NULL;
  if (gzip == false) ret = inflateInit2(&strm,15);
  else               ret = inflateInit2(&strm,31); 
  if (ret != Z_OK) return ret;
  /* decompress until deflate stream ends or end of file */
  do {
    strm.avail_in = source.sread(in,CHUNK);
    //if (ferror(source)) {(void)inflateEnd(&strm); return Z_ERRNO;}
    if (strm.avail_in == 0)  break;
    strm.next_in = in;
    /* run inflate() on input until output buffer not full */
    do {
      strm.avail_out = CHUNK;
      strm.next_out = out;
      ret = inflate(&strm, Z_NO_FLUSH);
      //assert(ret != Z_STREAM_ERROR); /* state not clobbered */
      switch (ret) {
      case Z_NEED_DICT:
        ret = Z_DATA_ERROR;   /* and fall through */
      case Z_DATA_ERROR:
      case Z_MEM_ERROR:
        (void)inflateEnd(&strm);
        return ret;
      }
      have = CHUNK - strm.avail_out;
      //if (fwrite(out, 1, have, dest) != have || ferror(dest)) { (void)inflateEnd(&strm); return Z_ERRNO;      }
      dest.swrite(out,have);
    } while (strm.avail_out == 0);
    /* done when inflate() says it's done */
  } while (ret != Z_STREAM_END);

  /* clean up and return */
  (void)inflateEnd(&strm);
  return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

}
