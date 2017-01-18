#ifndef BASE_H
#define BASE_H

#define BD_NAME "dig_app_text.cb"
#define DB_HEADER_NB_VALUES 5
#define DB_CHAR_WIDTH 28
#define DB_CHAR_HEIGHT 28
#define DB_NB_CLASS    10

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>
#include <unistd.h>

#define PUT(c) (file.put((char)(c)))
#define PUT_(c) (file.put(255-(char)(c)))

void randomInit(void) {srand(getpid());}
double randomDouble(void) {return rand()/(RAND_MAX+1.0);}
unsigned char randomUChar(void) {return (unsigned char)(256.0*rand()/(RAND_MAX+1.0));}

namespace uci {

  template<typename T>
  class Nop {
  public:
    void operator()(T& t) {}
  };

  template<int WIDTH,int HEIGHT,
	   typename CONTENT,
	   typename RANDOM = Nop<CONTENT> >
  class Imagette {

  public :
    

  private :

    CONTENT _content[WIDTH*HEIGHT];
    
  public :

    enum {
      width  = WIDTH,
      height = HEIGHT,
      size   = WIDTH*HEIGHT
    };

    Imagette(void) {
      int k;
      RANDOM random;

      for(k=0;k<WIDTH*HEIGHT;k++)
	random(_content[k]);
    }

    Imagette(const Imagette& i) {
      int k;

      for(k=0;k<WIDTH*HEIGHT;k++)
	_content[k]=i._content[k];
      
    }
    
    ~Imagette(void) {}

    Imagette& operator=(const Imagette& i) {
      int k;

      if(&i != this) 
	for(k=0;k<WIDTH*HEIGHT;k++)
	  _content[k]=i._content[k];

      return *this;
    }

    CONTENT& operator()(int h, int w) {
      return _content[w+h*WIDTH];
    }

    const CONTENT& operator()(int h, int w) const {
      return _content[w+h*WIDTH];
    }

    void PPM(std::string file_name,int no) {
      std::ostringstream os;
      std::ofstream file;

      os << file_name << '-' 
	 << std::setw(6) << std::setfill('0') << no << ".ppm";
      file.open(os.str().c_str());
      if(!file) {
	std::cerr << "Error : uci::PPM : "
		  << "I cannot open \"" 
		  << os.str().c_str() 
		  << "\". I exit."
		  << std::endl;
	::exit(1);
      }

      file << "P5\n" 
	   << width 
	   << ' ' 
	   << height 
	   << "\n255\n";

      for(int k=0;k<width*height;++k)
	PUT_(_content[k]);

      file.close();

      std::cout << "Image \"" << os.str() << "\" generated." << std::endl;
    }

  };


  class UCharRandom {
  public:

    void operator()(unsigned char& c) {
      c = randomUChar();
    }
  };

  class DoubleRandom {
  public:

    void operator()(double& c) {
      c = 255*randomDouble();
    }
  };

  


  template<int WIDTH,int HEIGHT,
	   int IMG_WIDTH, int IMG_HEIGHT>
  class Map : public Imagette<WIDTH,HEIGHT,
			      Imagette<IMG_WIDTH, IMG_HEIGHT,
				       double, DoubleRandom> > {

  public:

    typedef Imagette<IMG_WIDTH, IMG_HEIGHT, double, DoubleRandom> imagette;
    enum {
      width  = WIDTH,
      height = HEIGHT,
      size   = WIDTH*HEIGHT
    };

    Map(void) {}
    ~Map(void) {}
      
    void PPM(std::string file_name,int no) {
      std::ostringstream os;
      std::ofstream file;
      int h,hh,w,ww;
      char c;

      os << file_name << '-' 
	 << std::setw(6) << std::setfill('0') << no << ".ppm";
      file.open(os.str().c_str());
      if(!file) {
	std::cerr << "Error : uci::Map::PPM : "
		  << "I cannot open \"" 
		  << os.str().c_str() 
		  << "\". I exit."
		  << std::endl;
	::exit(1);
      }

      file << "P6\n" 
	   << WIDTH*(IMG_WIDTH+1)+1 
	   << ' ' 
	   << HEIGHT*(IMG_HEIGHT+1)+1 
	   << "\n255\n";
      
      // Blue line.
      PUT(0); PUT(0); PUT(255);
      for(w=0;w<WIDTH;w++) {
	for(ww=0;ww<IMG_WIDTH;ww++) {
	  PUT(0); PUT(0); PUT(255);
	}
	PUT(0); PUT(0); PUT(255);
      }
	
      for(h=0;h<HEIGHT;h++)
	{
	  for(hh=0;hh<IMG_HEIGHT;hh++)
	    {
	      PUT(0); PUT(0); PUT(255);
	      for(w=0;w<WIDTH;w++) {
		for(ww=0;ww<IMG_WIDTH;ww++) {
		  c=255-(unsigned char)(((*this)(h,w))(hh,ww)+.5);
		  PUT(c); PUT(c); PUT(c);
		}
		PUT(0); PUT(0); PUT(255);
	      }
	    }
	  
	  // End width blue line.
	  PUT(0); PUT(0); PUT(255);
	  for(w=0;w<WIDTH;w++) {
	    for(ww=0;ww<IMG_WIDTH;ww++) {
	      PUT(0); PUT(0); PUT(255);
	    }
	    PUT(0); PUT(0); PUT(255);
	  }
	}
      

      file.close();
      std::cout << "Image file (map) \"" << os.str() << "\" generated." << std::endl;
    }
  };
	   

  class Database {
  private:

    std::ifstream file;

    void ReOpen(void) {
      int i,v;

      file.clear();
      file.open(BD_NAME);
      if(!file)
	{
	  std::cerr << "Cannot open database. Aborting" << std::endl;
	  ::exit(1);
	}

      for(i=0;i<DB_HEADER_NB_VALUES;file >> v, i++);
    }

  public:

    typedef Imagette<DB_CHAR_WIDTH,DB_CHAR_HEIGHT, unsigned char, UCharRandom> imagette;
    imagette input;
    int      what;

    Database(void) {
      ReOpen();
    }

    ~Database(void) {}

    void Next(void) {

      int i,j,v;

      for(i=0;i<DB_CHAR_HEIGHT;i++)
	for(j=0;j<DB_CHAR_WIDTH;j++)
	  {
	    file >> v;
	    input(i,j) = (unsigned char)v;
	  }

      if(file.eof())
	{
	  file.close();
	  ReOpen();
	  Next();
	}
      else
	for(i=0,what=-1;i<DB_NB_CLASS;i++)
	  {
	    file >> v;
	    if(v==1)
	      what=i;
	  }
    }

    
  };

}


#endif

