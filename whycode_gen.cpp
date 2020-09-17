#include <stdlib.h>
#include <math.h>
#include "CNecklace.h"

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <bitset>


// Define default parameters
bool verbose = false;
bool legacy = false;
int hamming = 1;
int xc = 900;
int yc = 900;
int index = 0;
int n;
float w;


// Display help
void display_help()
{
  std::cout << "Usage: whycon-id-gen [-v] -l\n" << \
               "       whycon-id-gen [-v] [-d <distance>] <bits>\n" << \
               "    -v,    Verbose while generating canvas (default: false)\n" << \
               "    -l,    Generating the original WhyCon marker\n" << \
               "    -d,    Set minimal Hamming distance (default: 1)\n";
}

std::string create_svg_open(const std::string width, const std::string height)
{
  return "<svg width=\"" + width + "\" height=\"" + height + "\" viewBox=\"0 0 1800 1800\" preserveAspectRatio=\"xMidYMid meet\" xmlns=\"http://www.w3.org/2000/svg\">";
}

std::string create_svg_close()
{
  return "</svg>";
}

std::string create_circle(const std::string cx, const std::string cy, const std::string r, const std::string fill, const std::string stroke)
{
  return "<circle cx=\"" + cx + "\" cy=\"" + cy + "\" r=\"" + r + "\" fill=\"" + fill + "\" stroke=\"" + stroke + "\"/>";
}

std::string create_polygon(const std::string fill, const std::string stroke, const std::string points)
{
  return "<polygon fill=\"" + fill + "\" stroke=\"" + stroke + "\" points=\"" + points + "\"/>";
}

std::string create_rect_bg(const std::string width, const std::string height, const std::string fill, const std::string stroke)
{
  return "<rect x=\"0\" y=\"0\" width=\"" + width + "\" height=\"" + height + "\" fill=\"" + fill + "\" stroke=\"" + stroke + "\"/>";
}

std::string create_text(const std::string x, const std::string y, const std::string fill, const std::string size, const std::string txt)
{
  return "<text x=\"" + x + "\" y=\"" + y + "\" fill=\"" + fill + "\" font-size=\"" + size + "\">" + txt + "</text>";
}

// Draw the encded ID into WhyCon marker
void draw_whycode_markers(int id, int idx, const int teethCount)
{
    if(verbose)
      std::cout << "Generating WhyCode Canvas for Id " << idx << "(encoding " << id <<")\n";

    std::ofstream ofs(std::to_string(idx) + ".svg", std::ofstream::trunc);
    if(!ofs.is_open())
    {
      std::cerr << "Error opening file\n";
      return;
    }
    ofs << create_svg_open("1800", "1800");
    ofs << create_rect_bg("1800", "1800", "white", "none");
    ofs << create_circle("900", "900", "899", "white", "black");
    ofs << create_circle("900", "900", "700", "black", "none");
    ofs << create_circle("900", "900", "420", "white", "none");

    // Convert lowest bit shift to binary
    if(verbose)
      std::cout << "Converting ID to binary\n";
    std::string s = std::bitset<32>(id).to_string();

    // For each encoding bit
    double x1, y1, x2, y2;
    for(int i = 0; i < teethCount; i++){
        // Calculate the pixel positions of each segment
        x1 = xc + 650 * cos(-w * (2 * i + (s.at(i + 32 - teethCount) - '0') * 2.0) / 180.0 * M_PI);
        y1 = yc + 650 * sin(-w * (2 * i + (s.at(i + 32 - teethCount) - '0') * 2.0) / 180.0 * M_PI);
        x2 = xc + 650 * cos(-w * (2 * i + 1) / 180.0 * M_PI);
        y2 = yc + 650 * sin(-w * (2 * i + 1) / 180.0 * M_PI);

        // Draw each of the segments onto the original WhyCon marker
        if(verbose)
          std::cout << "Drawing Segment Size: " << x1 << "," << y1 << " " << x2 << "," << y2 << std::endl;
        ofs << create_polygon("black", "none", "900,900 " + std::to_string(x1) + "," + std::to_string(y1) + " " + std::to_string(x2) + "," + std::to_string(y2));
    }

    // Draw a final white circle in the centre to complete the marker
    std::cout << "Rendering final image: " << idx << " (encoding " << id << ") => " << idx << ".svg\n";
    ofs << create_circle("900", "900", "240", "white", "none");
    ofs << create_text("1600", "100", "black", "44", "ID: " + std::to_string(idx));
    ofs << create_svg_close();
    ofs.close();
}

int main(int argc, char *argv[])
{
  if(argc == 1)
  {
    std::cerr << "Not enough arguments\n";
    display_help();
    return 1;
  }

  int c;
  while((c = getopt(argc, argv, "hd:vl")) != -1)
  {
      switch(c)
      {
          case 'h':
            display_help();
            return 0;
          case 'd':
            if(isdigit(optarg[0]))
            {
              hamming = atoi(optarg);
            }
            else
            {
              std::cerr << "Invalid Hamming distance\n";
              display_help();
              return 1;
            }
            break;
          case 'v':
            verbose = true;
            break;
          case 'l':
            legacy = true;
            break;
          default:
          case '?':
            std::cerr << "Unknown option\n";
            display_help();
            return 1;
      }
  }

  if(optind > argc + 1)
  {
    std::cerr << "Too many arguments\n";
    display_help();
    return 1;
  }

  if(legacy)
  {
    if(verbose)
      std::cout << "Generate original WhyCon marker\n";

    std::ofstream ofs("00000000.svg", std::ofstream::trunc);
    if(ofs.is_open())
    {
      ofs << create_svg_open("1800", "1800");
      ofs << create_rect_bg("1800", "1800", "white", "none");
      ofs << create_circle("900", "900", "899", "white", "black");
      ofs << create_circle("900", "900", "700", "black", "none");
      ofs << create_circle("900", "900", "420", "white", "none");
      ofs << create_svg_close();
      ofs.close();
      std::cout << "Rendering final image: => 00000000.svg\n";
      return 0;
    }
    else
    {
      std::cerr << "Error opening file\n";
      return 1;
    }
  }

  if(!isdigit(argv[optind][0]))
  {
      std::cerr << "Invalid number of bits\n";
      display_help();
      return 1;
  }

  const int teethCount = atoi(argv[optind]);
  CNecklace decoder = CNecklace(teethCount, hamming);
  int a[10000];
  n = decoder.printAll(a);
  if (decoder.verifyHamming(a,teethCount,n) < hamming)
  {
      std::cerr << "Hamming distance too low!\n";
      return 1;
  }
  w = 360.0/(float)teethCount/2.0;

  for(int i = 0; i < n; i++)
  {
      draw_whycode_markers(a[i], i + 1, teethCount);
  }

  return 0;
}

