#ifndef __SMF_CACHEDGLYPH_HH
#define __SMF_CACHEDGLYPH_HH

class CachedGlyph
  {
  public:

    explicit CachedGlyph(CachedGlyphImpl* p = 0) : glyph_pointer_data(0)
    {
      if (p) glyph_pointer_data = new GlyphPointer(p);
    }
    ~CachedGlyph()
    {
      release();
    }

    CachedGlyph(const CachedGlyph& r) throw() : glyph_pointer_data(0)
    {
      acquire(r.glyph_pointer_data);
    }
    CachedGlyph& operator=(const CachedGlyph& other)
    {
      if (this != &other)
        {
          release();
          acquire(other.glyph_pointer_data);
        }
      return *this;
    }

    bool operator==(const CachedGlyph& other)
    {
      return(other.glyph_pointer_data == glyph_pointer_data);
    }

    bool operator!=(const CachedGlyph& other)
    {
      return(!operator==(other));
    }
    CachedGlyphImpl& operator*()  const throw()
    {
      return *glyph_pointer_data->ptr;
    }
    CachedGlyphImpl* operator->() const throw()
    {
      return glyph_pointer_data->ptr;
    }
    CachedGlyphImpl* get()        const throw()
    {
      return glyph_pointer_data ? glyph_pointer_data->ptr : 0;
    }
    bool unique() const throw()
    {
      return (glyph_pointer_data ? glyph_pointer_data->count == 1 : true);
    }

  private:

    struct GlyphPointer
      {
        GlyphPointer(CachedGlyphImpl* p = 0, unsigned c = 1) : ptr(p), count(c) {}
        CachedGlyphImpl*          ptr __attribute__ ((aligned));
        unsigned    count;
      }* glyph_pointer_data __attribute__ ((aligned));

    void acquire(GlyphPointer* c) throw()
    { // increment the count
      glyph_pointer_data = c;
      if (c) ++c->count;
    }

    void release()
    { // decrement the count, delete if it is 0
      if (glyph_pointer_data)
        {
          if (--glyph_pointer_data->count == 0)
            {
              if (glyph_pointer_data->ptr->bitmap.buffer)
                {
                  delete[](glyph_pointer_data->ptr->bitmap.buffer);
                  glyph_pointer_data->ptr->bitmap.buffer = 0;
                }
              delete glyph_pointer_data->ptr;
              delete glyph_pointer_data;
            }
          glyph_pointer_data = 0;
        }
    }
  };

#endif // __SMF_CACHEDGLYPH_H
