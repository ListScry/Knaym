#ifndef _IMAGE_
#define _IMAGE_

struct pixel {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

class Image {
  unsigned int width, height, range;
  pixel ** data;
public:
  Image (const char *);
  Image();
  ~Image ();
  void write(const char *);
  Image & operator-(const Image &);
};

#endif  // _IMAGE_
