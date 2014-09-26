#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

#include <bmp.h>


/*!
 * \brief writeBitmap a function that takes a matrix of colors and writes them into bitmap
 * \param data
 * \param fileName
 */
void writeBitmap( const std::vector< std::vector<Color> >& data, const std::string& fileName)
{
    unsigned char pad[3] = {0,0,0}; //need for padding 1 to 3 zero elements
    std::vector<unsigned char> color_pixels;
    for(unsigned int row = 0; row<data.size(); ++row)
        for( int col = 0; col < data[0].size(); ++col)
        {

            color_pixels.push_back(data[row][col].b); //BlUE
            color_pixels.push_back(data[row][col].g); //GREEN
            color_pixels.push_back(data[row][col].r); //RED
        }

    int pad_size = (4 - 3 * data[0].size() % 4) % 4;

    BMP bmp;
    size_t pixelCount = data.size()*data[0].size();
    bmp.DIBHeader.bitmapSizeInBytes = pixelCount * 3 + data.size() * pad_size;
    bmp.FileHeader.fileSizeInBytes = bmp.FileHeader.pixelArrayOffsetInBytes + bmp.DIBHeader.bitmapSizeInBytes;
    bmp.DIBHeader.bitmapWidthInPixels = static_cast< uint32_t >( data[0].size() );
    bmp.DIBHeader.bitmapHeightInPixels = static_cast< uint32_t >( data.size() );

    std::ofstream file;
    file.open (fileName.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
    if ( !file.is_open() ) {
        std::runtime_error("Failed to open '" + fileName + "' for writing");
    }
    file.write ((char*)(&bmp), sizeof(BMP));

    int position = color_pixels.size();
    while (position != 0)
    {
        for (unsigned int n = 0; n != data[0].size() * 3; n++)
        {
            unsigned char* temp = &color_pixels[position - data[0].size() * 3 + n];
            file.write ((char*)temp, 1);
        }
        file.write ((char*)(pad), pad_size);
        position = position - data[0].size() * 3;
    }
    file.flush();
    file.close();
}


/*!
 * \brief drawCircle takes a rectangle where to draw and draws a circle using basic circle equation
 * \param start point of rectangle where the drawing will be made
 * \param end point of rectangle where the drawing will be made
 * \param r radius of circle
 * \param center coords of center of the circle
 * \param data matrix of colors which will be written to bitmap
 * \param colors pair of colors which is used for background and inner filling
 */
void drawCircle(const std::pair<int, int> start, const std::pair<int, int> end, const int r,
                const std::pair<int, int> center, std::vector<std::vector<Color> > &data,
                const std::pair<Color, Color> &colors)
{
    const int a = center.first;
    const int b = center.second;
    for(int i = start.first; i < end.first; ++i)
    {
        for(int j = start.second; j < end.second; ++j)
        {
            if(r * r < (i - a) * (i - a) + (j - b) * (j - b))
            {
                data[i][j] = colors.first;
            }
            else
            {
                data[i][j] = colors.second;
            }
        }
    }
}

/*!
 * \brief drawSquare takes a rectangle where to draw and draws a square
 * \param start point of rectangle where the drawing will be made
 * \param end point of rectangle where the drawing will be made
 * \param a length of square side
 * \param center coords of center of the square
 * \param data matrix of colors which will be written to bitmap
 * \param colors pair of colors which is used for background and inner filling
 */
void drawSquare(const std::pair<int, int> start, const std::pair<int, int> end, const int a,
                const std::pair<int, int> center, std::vector<std::vector<Color> > &data,
                const std::pair<Color, Color> &colors)
{
    for(int i = start.first; i < end.first; ++i)
    {
        for(int j = start.second; j < end.second; ++j)
        {
            if( (i > center.first - a / 2 && i < center.first + a / 2) &&
                (j > center.second - a / 2 && j < center.second + a / 2) )
            {
                data[i][j] = colors.first;
            }
            else
            {
                data[i][j] = colors.second;
            }

        }
    }
}

/*!
 * \brief drawTriangle takes a rectangle where to draw and draws a triangle using common algorithm for drawing filled triangles
 * \param start point of rectangle where the drawing will be made
 * \param end point of rectangle where the drawing will be made
 * \param a length of triangle leg
 * \param center coords of center of the square
 * \param data matrix of colors which will be written to bitmap
 * \param colors pair of colors which is used for background and inner filling
 */
void drawTriangle(const std::pair<int, int> start, const std::pair<int, int> end, const int a,
                  const std::pair<int, int> center, std::vector<std::vector<Color> > &data,
                  const std::pair<Color, Color> &colors)
{
    //filing background
    for(int i = start.first; i < end.first; ++i)
    {
        for(int j = start.second; j < end.second; ++j)
        {
            data[i][j] = colors.second;
        }
    }

    /* calculating A, B, C points.

        A         A = (x1, y1)
       / \        B = (x2, y2)
      B - C       C = (x3, y3)

    */
    float x1 = center.first, x2 = center.first - a / 2, x3 = center.first + a / 2;
    float y1 = center.second - a / 2, y2 = center.second + a / 2, y3 = center.second + a / 2;

    //calculating inverse slope of left and right leg
    float invslope1 = (x2 - x1) / (y2 - y1);
    float invslope2 = (x3 - x1) / (y3 - y1);

    float curx1 = x1;
    float curx2 = x1;

    for (int scanlineY = y1; scanlineY <= y2; ++scanlineY)
    {
        //drawing a horizontal line with filling inside triangle
        for(int i = (int)curx1; i < (int)curx2; ++i)
        {
            data[scanlineY][i] = colors.first;
        }
        curx1 += invslope1;
        curx2 += invslope2;
    }

}

/*!
 * \brief drawDiamond takes a rectangle where to draw and draws a diamond using common algorithm for drawing filled triangles
 * In fact draws two triangles one is flat bottom triangle and second is flat top triangle.
 * \param start point of rectangle where the drawing will be made
 * \param end point of rectangle where the drawing will be made
 * \param a length of diamond side
 * \param center coords of center of the square
 * \param data matrix of colors which will be written to bitmap
 * \param colors pair of colors which is used for background and inner filling
 */
void drawDiamond(const std::pair<int, int> start, const std::pair<int, int> end, const int a,
                  const std::pair<int, int> center, std::vector<std::vector<Color> > &data,
                  const std::pair<Color, Color> &colors)
{
    for(int i = start.first; i < end.first; ++i)
    {
        for(int j = start.second; j < end.second; ++j)
        {
            data[i][j] = colors.second;
        }
    }

    /* calculating A, B, C points.

        A         A = (x1, y1)
       / \        B = (x2, y2)
      B - C       C = (x3, y3)

    */
    float x1 = center.first, x2 = center.first - a / 2, x3 = center.first + a / 2;
    float y1 = center.second - a / 2, y2 = center.second, y3 = center.second;

    float invslope1 = (x2 - x1) / (y2 - y1);
    float invslope2 = (x3 - x1) / (y3 - y1);


    float curx1 = x1;
    float curx2 = x1;

    for (int scanlineY = y1; scanlineY <= y2; ++scanlineY)
    {
        for(int i = (int)curx1; i < (int)curx2; ++i)
        {
            data[scanlineY][i] = colors.first;
        }
        curx1 += invslope1;
        curx2 += invslope2;
    }

    /* calculating A, B, C points.

      A - B      A = (x1, y1)
       \ /       B = (x2, y2)
        C        C = (x3, y3)

    */
    x1 = center.first - a / 2, x2 = center.first + a / 2, x3 = center.first;
    y1 = center.second, y2 = center.second, y3 = center.second + a / 2;


    invslope1 = (x3 - x1) / (y3 - y1);
    invslope2 = (x3 - x2) / (y3 - y2);

    curx1 = x3;
    curx2 = x3;

    for (int scanlineY = y3; scanlineY > y1; --scanlineY)
    {
        curx1 -= invslope1;
        curx2 -= invslope2;

        for(int i = (int)curx1; i < (int)curx2; ++i)
        {
            data[scanlineY][i] = colors.first;
        }
    }
}



int main()
{
    using namespace std;
    vector<vector<Color> > data;
    Color filling[4] = { {255, 213, 43}, {31, 113, 236}, {75, 249, 48}, {255, 0, 0} };
    Color background[4] = { {0, 0, 0}, {236, 127, 31}, {235, 126, 247}, {26, 206, 176} };
    const int N = 400;
    const int M = 600;
    data.resize(N);
    for(int i = 0; i < N; ++i)
    {
        data[i].resize(M);
    }

    drawCircle(make_pair(0, 0), make_pair(N / 2, M / 2), 75, make_pair(N / 4 , M / 4), data, make_pair(filling[0], background[0]));

    drawTriangle(make_pair(0, M / 2), make_pair(N / 2, M), 150, make_pair(M / 2 + M / 4 , N / 4), data, make_pair(filling[1], background[1]));

    drawDiamond(make_pair(N / 2, 0), make_pair(N, M / 2), 150, make_pair(M / 4 , N / 4 + N / 2), data, make_pair(filling[2], background[2]));

    drawSquare(make_pair(N / 2, M / 2), make_pair(N, M), 150, make_pair(N / 2 + N / 4 , M / 2 + M / 4), data, make_pair(filling[3], background[3]));

    writeBitmap(data, "output.bmp");
    return 0;
}

