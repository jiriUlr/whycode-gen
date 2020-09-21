#include <stdlib.h>

#include "CNecklace.h"

#include <unistd.h>

#include <array>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>


// Define default parameters
bool verbose = false;
bool legacy = false;
int hamming = 1;
const int xc = 900;
const int yc = 900;
float w;

struct Point
{
  double x;
  double y;
};

// Display help
void display_help()
{
  std::cout << "Usage: whycon-id-gen [-v] -l\n" <<
               "       whycon-id-gen [-v] [-d <distance>] <bits>\n\n" <<
               "    -v,    Verbose while generating canvas (default: false)\n" <<
               "    -l,    Generating the original WhyCon marker\n" <<
               "    -d,    Set minimal Hamming distance (default: 1)\n";
}

std::string start_svg(const std::string width, const std::string height)
{
  return "<svg width=\"" + width + "\" height=\"" + height + "\" viewBox=\"0 0 1800 1800\"" +
         " preserveAspectRatio=\"xMidYMid meet\" xmlns=\"http://www.w3.org/2000/svg\">" +
         "<rect x=\"0\" y=\"0\" width=\"1800\" height=\"1800\" fill=\"white\" stroke=\"none\"/>" +
         "<circle cx=\"900\" cy=\"900\" r=\"899\" fill=\"white\" stroke=\"black\"/>" +
         "<circle cx=\"900\" cy=\"900\" r=\"700\" fill=\"black\" stroke=\"none\"/>" +
         "<circle cx=\"900\" cy=\"900\" r=\"420\" fill=\"white\" stroke=\"none\"/>";
}

std::string end_svg()
{
  return "<circle cx=\"900\" cy=\"900\" r=\"240\" fill=\"white\" stroke=\"none\"/></svg>";
}

std::string draw_segments(const std::vector<std::array<Point, 2>>& points)
{
  std::ostringstream oss("<path fill=\"black\" stroke=\"none\" d=\"M900,900 L", std::ostringstream::ate);
  for(auto&& arr : points)
    oss << arr[0].x << "," << arr[0].y << " " << arr[1].x << "," << arr[1].y << " 900,900 ";
  oss << "Z\"/>";
  return oss.str();
}

std::string write_text(const std::string size, const std::string txt)
{
  return "<text x=\"1600\" y=\"100\" fill=\"black\" font-size=\"" + size + "\">" + txt + "</text>";
}

// Draw the encded ID into WhyCon marker
void draw_whycode_markers(int id, int idx, const int teethCount)
{
  if(verbose)
    std::cout << "Generating WhyCode for ID " << idx << "(encoding " << id << ")\n";

  std::ofstream ofs(std::to_string(idx) + ".svg", std::ofstream::trunc);
  if(!ofs.is_open())
  {
    std::cerr << "Error opening file\n";
    return;
  }
  ofs << start_svg("200mm", "200mm");

  // Convert lowest bit shift to binary
  std::string s = std::bitset<32>(id).to_string();

  // For each encoding bit calculate the pixel positions of each segment
  double x1, y1, x2, y2;
  std::vector<std::array<Point, 2>> points(teethCount);
  for(int i = 0; i < teethCount; i++)
  {
    x1 = xc + 650 * std::cos(-w * (2 * i + (s.at(i + 32 - teethCount) - '0') * 2.0) / 180.0 * M_PI);
    y1 = yc + 650 * std::sin(-w * (2 * i + (s.at(i + 32 - teethCount) - '0') * 2.0) / 180.0 * M_PI);
    x2 = xc + 650 * std::cos(-w * (2 * i + 1) / 180.0 * M_PI);
    y2 = yc + 650 * std::sin(-w * (2 * i + 1) / 180.0 * M_PI);

    if(verbose)
      std::cout << "Drawing Segment Size: " << x1 << "," << y1 << " " << x2 << "," << y2 << std::endl;
    points[i] = std::array<Point, 2>{Point{x1, y1}, Point{x2, y2}};
  }
  ofs << draw_segments(points);

  // Draw a final white circle in the centre to complete the marker
  std::cout << "Rendering final image: " << idx << " (encoding " << id << ") => " << idx << ".svg\n";
  ofs << write_text("44", "ID: " + std::to_string(idx));
  ofs << end_svg();
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

    std::ofstream ofs("whycon.svg", std::ofstream::trunc);
    if(!ofs.is_open())
    {
      std::cerr << "Error opening file\n";
      return 1;
    }
    ofs << start_svg("200mm", "200mm");
    ofs << end_svg();
    ofs.close();
    std::cout << "Rendering final image: => whycon.svg\n";
    return 0;
  }

  if(!isdigit(argv[optind][0]) || atoi(argv[optind]) < 3)
  {
    std::cerr << "Invalid number of bits. Accepts only >= 3\n";
    display_help();
    return 1;
  }

  const int teethCount = atoi(argv[optind]);
  CNecklace decoder = CNecklace(teethCount, hamming);
  int a[10000];
  int n = decoder.printAll(a);
  if (decoder.verifyHamming(a,teethCount, n) < hamming)
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

