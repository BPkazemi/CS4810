#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// allows the use of min() and max() functions
#include <algorithm>
#include <float.h>
#include <list>
using namespace std;

Pixel::Pixel(const Pixel32& p)
{
    // Convert RGB values of 0-255 to floating values of 0-1
    r = (float) p.r / 255.0f;
    g = (float) p.g / 255.0f;
    b = (float) p.b / 255.0f;
    a = (float) p.a / 255.0f;
}

Pixel32::Pixel32(const Pixel& p)
{
    // Convert values in 0-1 to RGB values of 0-255
    r = (unsigned char) max( 0.0f, min(255.0f, p.r * 255.0f) );
    g = (unsigned char) max( 0.0f, min(255.0f, p.g * 255.0f) );
    b = (unsigned char) max( 0.0f, min(255.0f, p.b * 255.0f) );
    a = (unsigned char) p.a;
}

/*
 * ===================== Helper functions ====================
 */
float genRand( int bits ) {

    // I've played around with noiseIntensity to get these constants to best mirror the sample outputs
    float noiseIntensity = 1.0 / pow( 2, (float) bits-1);  // Larger bit depth = Smaller intensity
    float randNum = noiseIntensity * ( (float) rand() / (float) RAND_MAX );
    return ( randNum - (0.5 * noiseIntensity) );
}

// Clamps a value to one of 2^b bit levels based off the pixel value
float quantize( float pixelValue, float threshold, int bits ) {
    int bitLevels = pow( 2, bits );
    float bitLevel = pixelValue * bitLevels;

    if ( bitLevel > threshold ) {
        bitLevel = ceil( bitLevel );
    } else {
        bitLevel = floor( bitLevel );
    }
    return bitLevel;
}


/*
 * ===================== Image Processing Functions ===================
 */
int Image32::AddRandomNoise(const float& noise,Image32& outputImage) const
{
    int seed = time(NULL);
    srand(seed);
    // Noise: from [-n, n] to match the expect output better
    for( int x = 0; x < outputImage.width(); x++ ) {
        for( int y = 0; y < outputImage.height(); y++ ) {
            Pixel curPixel = *(new Pixel( outputImage.pixel( x, y ) ));

            curPixel.r = curPixel.r + 
                ( ((((float) rand() / (float) RAND_MAX) * noise*2) - (noise)) );
            curPixel.g = curPixel.g + 
                ( ((((float) rand() / (float) RAND_MAX) * noise*2) - (noise)) );
            curPixel.b = curPixel.b + 
                ( ((((float) rand() / (float) RAND_MAX) * noise*2) - (noise)) );

            outputImage.pixel( x, y ) = *( new Pixel32( curPixel ) );
        }
    }
	return 1;
}

int Image32::Brighten(const float& brightness,Image32& outputImage) const
{
    for( int x = 0; x < outputImage.width(); x++ ) {
        for( int y = 0; y < outputImage.height(); y++  ) {
            Pixel curPixel = *( new Pixel( outputImage.pixel( x, y ) ));
            curPixel.r = curPixel.r * brightness;
            curPixel.g = curPixel.g * brightness;
            curPixel.b = curPixel.b * brightness;

            outputImage.pixel( x, y ) = *(new Pixel32( curPixel ));
        }
    }
    return 1;
}

int Image32::Luminance(Image32& outputImage) const
{
    unsigned char wAverage;
    for( int x = 0; x < outputImage.width(); x++ ) {
        for( int y = 0; y < outputImage.height(); y++  ) {
            Pixel32 curPixel = outputImage.pixel( x, y );
            
            wAverage = (0.30 * curPixel.r + 0.59 * curPixel.g + 0.11 * curPixel.b);
            outputImage.pixel( x, y ).r = wAverage;
            outputImage.pixel( x, y ).g = wAverage;
            outputImage.pixel( x, y ).b = wAverage;
        }
    }
	return 1;
}

int Image32::Contrast(const float& contrast,Image32& outputImage) const
{
    double averageLum = 0;
    double totalLum = 0;
    for ( int x = 0; x < outputImage.width(); x++ ) {
        for ( int y = 0; y < outputImage.height(); y++ ) {
            Pixel curPixel = *( new Pixel( outputImage.pixel( x, y ) ));
            totalLum += (0.30 * curPixel.r + 0.59 * curPixel.g + 0.11 * curPixel.b);
        }
    }

    averageLum = totalLum / (outputImage.width() * outputImage.height());

    double pixelLum = 0;
    for ( int x = 0; x < outputImage.width(); x++ ) {
        for ( int y = 0; y < outputImage.height(); y++ ) {
            Pixel curPixel = *( new Pixel( outputImage.pixel( x, y ) ));
            pixelLum = (0.30 * curPixel.r + 0.59 * curPixel.g + 0.11 * curPixel.b);

            curPixel.r = averageLum + (contrast * (curPixel.r - averageLum));
            curPixel.g = averageLum + (contrast * (curPixel.g - averageLum));
            curPixel.b = averageLum + (contrast * (curPixel.b - averageLum));

            outputImage.pixel( x, y ) = *( new Pixel32(curPixel) );
        }
    }
	return 1;
}

int Image32::Saturate(const float& saturation,Image32& outputImage) const
{
    double pixelLum = 0;
    for ( int x = 0; x < outputImage.width(); x++ ) {
        for ( int y = 0; y < outputImage.height(); y++ ) {
            Pixel curPixel = *( new Pixel( outputImage.pixel( x, y )));
            pixelLum = (0.30 * curPixel.r + 0.59 * curPixel.g + 0.11 * curPixel.b);

            curPixel.r = pixelLum + (saturation * (curPixel.r - pixelLum));
            curPixel.g = pixelLum + (saturation * (curPixel.g - pixelLum));
            curPixel.b = pixelLum + (saturation * (curPixel.b - pixelLum));

            outputImage.pixel( x, y ) = *( new Pixel32( curPixel ));
        }
    }
	return 1;
}

int Image32::Quantize(const int& bits,Image32& outputImage) const
{
    for ( int x = 0; x < outputImage.width(); x++ ) {
        for ( int y = 0; y < outputImage.height(); y++ ) {
            Pixel curPixel = *( new Pixel( outputImage.pixel( x, y ) ));

            int bitLevels = pow( 2, bits );
            float threshold = bitLevels / 2.0;
            float bitLevelR = quantize( curPixel.r, threshold, bits ); 
            float bitLevelG = quantize( curPixel.g, threshold, bits ); 
            float bitLevelB = quantize( curPixel.b, threshold, bits ); 

            curPixel.r = ( bitLevelR / bitLevels ) * 1.0;
            curPixel.g = ( bitLevelG / bitLevels ) * 1.0;
            curPixel.b = ( bitLevelB / bitLevels ) * 1.0;

            outputImage.pixel( x, y ) = *( new Pixel32( curPixel ));
        }
    }
	return 1;
}

int Image32::RandomDither(const int& bits,Image32& outputImage) const
{
    for ( int x = 0; x < outputImage.width(); x++ ) {
        for ( int y = 0; y < outputImage.height(); y++ ) {
            Pixel curPixel = *( new Pixel( outputImage.pixel( x, y )));

            // 1. Add noise
            curPixel.r = curPixel.r + genRand( bits );
            curPixel.g = curPixel.g + genRand( bits );
            curPixel.b = curPixel.b + genRand( bits );

            // 2. Quantize
            int bitLevels = pow( 2, bits );
            float threshold = bitLevels / 2.0;
            float bitLevelR = quantize( curPixel.r, threshold, bits ); 
            float bitLevelG = quantize( curPixel.g, threshold, bits ); 
            float bitLevelB = quantize( curPixel.b, threshold, bits ); 

            curPixel.r = bitLevelR / bitLevels;
            curPixel.g = bitLevelG / bitLevels;
            curPixel.b = bitLevelB / bitLevels;

            outputImage.pixel( x, y ) = *( new Pixel32( curPixel ));
        }
    }
	return 1;
}
int Image32::OrderedDither2X2(const int& bits,Image32& outputImage) const
{
    // TODO: Result pattern bands look *subtly* off
    float mask[ 2 ][ 2 ] = {{1.0/5.0, 3.0/5.0},
                            {4.0/5.0, 2.0/5.0}};
    for ( int x = 0; x < outputImage.width(); x++ ) {
        for ( int y = 0; y < outputImage.height(); y++ ) {
            Pixel curPixel = *( new Pixel( outputImage.pixel( x, y )));
            int i = x % 2;
            int j = y % 2;
            
            int bitLevels = pow(2, bits) - 1;
            float bitLevelR = curPixel.r * (float) bitLevels;
            float bitLevelG = curPixel.g * (float) bitLevels;
            float bitLevelB = curPixel.b * (float) bitLevels;

            float rErr = bitLevelR - (float) floor( bitLevelR );
            float gErr = bitLevelG - (float) floor( bitLevelG );
            float bErr = bitLevelB - (float) floor( bitLevelB );

            if ( rErr > (mask[i][j]) ) {
                bitLevelR = (float) ceil( bitLevelR ); 
            } else {
                bitLevelR = (float) floor( bitLevelR );
            }
            if ( gErr > (mask[i][j]) ) {
                bitLevelG = (float) ceil( bitLevelG );
            } else {
                bitLevelG = (float) floor( bitLevelG );
            }
            if ( bErr > (mask[i][j]) ) {
                bitLevelB = (float) ceil( bitLevelB );
            } else {
                bitLevelB = (float) floor( bitLevelB );
            }

            curPixel.r = (float) bitLevelR / (float) bitLevels;
            curPixel.g = (float) bitLevelG / (float) bitLevels;
            curPixel.b = (float) bitLevelB / (float) bitLevels;

            outputImage.pixel( x, y ) = *( new Pixel32( curPixel ) );
        }
    }
	return 1;
}


int Image32::FloydSteinbergDither(const int& bits,Image32& outputImage) const
{
    // TODO: Result patterns are off, but the algorithm looks correct 

    Image32 destination = *( new Image32() );
    destination.setSize( outputImage.width(), outputImage.height() );

    for ( int x = 0; x < outputImage.width(); x++ ) {
        for ( int y = 0; y < outputImage.height(); y++ ) {
            Pixel curPixel = *( new Pixel( outputImage.pixel( x, y )));

            // 1. Quantize
            int bitLevels = pow( 2, bits );
            float threshold = bitLevels / 2.0;

            float bitLevelR = quantize( curPixel.r, threshold, bits ); 
            float bitLevelG = quantize( curPixel.g, threshold, bits ); 
            float bitLevelB = quantize( curPixel.b, threshold, bits ); 

            float newR = ( bitLevelR / bitLevels );
            float newG = ( bitLevelG / bitLevels );
            float newB = ( bitLevelB / bitLevels );

            float rErr = curPixel.r - newR;
            float gErr = curPixel.g - newG;
            float bErr = curPixel.b - newB;

            curPixel.r = newR;
            curPixel.g = newG;
            curPixel.b = newB;

            destination.pixel( x, y ) = *( new Pixel32( curPixel ));

            // 2. Diffuse errors
            float alpha = 7.0 / 16.0;
            float beta = 3.0 / 16.0;
            float gamma = 5.0 / 16.0;
            float delta = 1.0 / 16.0;

            if ( y + 1 < outputImage.height() ) {
                Pixel neighborPixel = *( new Pixel( outputImage.pixel(x, y+1)));
                neighborPixel.r = neighborPixel.r + alpha * rErr;
                neighborPixel.g = neighborPixel.g + alpha * gErr;
                neighborPixel.b = neighborPixel.b + alpha * bErr;

                outputImage.pixel( x, y+1 ) = *( new Pixel32( neighborPixel ) );
            }
            if ( x + 1 < outputImage.width() ) {
                Pixel neighborPixel = *( new Pixel( outputImage.pixel(x+1, y)));
                neighborPixel.r = neighborPixel.r + gamma * rErr;
                neighborPixel.g = neighborPixel.g + gamma * gErr;
                neighborPixel.b = neighborPixel.b + gamma * bErr;

                outputImage.pixel( x+1, y ) = *( new Pixel32( neighborPixel ) );

                if ( y - 1 >= 0 ) {
                    Pixel neighborPixel = *( new Pixel( outputImage.pixel(x+1, y-1)));
                    neighborPixel.r = neighborPixel.r + beta * rErr;
                    neighborPixel.g = neighborPixel.g + beta * gErr;
                    neighborPixel.b = neighborPixel.b + beta * bErr;

                    outputImage.pixel( x+1, y-1 ) = *( new Pixel32( neighborPixel ) );
                }

                if ( y + 1 < outputImage.height() ) {
                    Pixel neighborPixel = *( new Pixel( outputImage.pixel(x+1, y+1)));
                    neighborPixel.r = neighborPixel.r + delta * rErr;
                    neighborPixel.g = neighborPixel.g + delta * gErr;
                    neighborPixel.b = neighborPixel.b + delta * bErr;

                    outputImage.pixel( x+1, y+1 ) = *( new Pixel32( neighborPixel ) );
                }
            }
        }
    }

    outputImage = destination;
	return 1;
}

int Image32::Blur3X3(Image32& outputImage) const
{
    double mask[3][3] = 
    {
        { 1.0/16.0, 2.0/16.0, 1.0/16.0 },
        { 2.0/16.0, 4.0/16.0, 2.0/16.0 },
        { 1.0/16.0, 2.0/16.0, 1.0/16.0 }
    };

    Image32 *destination = new Image32();
    destination->setSize(outputImage.width(), outputImage.height());

    for ( int x = 0; x < outputImage.width(); x++ ) {
        for ( int y = 0; y < outputImage.height(); y++ ) {
            float accumR = 0;
            float accumG = 0;
            float accumB = 0;
            float wSum = 0;
            for ( int xm = -1; xm <=1; xm++ ) {
                for ( int ym = -1; ym <= 1; ym++ ) {

                    // Ignore the edges! 
                    if ( x + xm < 0 || 
                            y + ym < 0 || 
                            x + xm == outputImage.width() || 
                            y + ym == outputImage.height() ) {
                        continue;
                    }

                    // Get the sums
                    Pixel curPixel = *( new Pixel( outputImage.pixel( x + xm, y + ym )));
                    accumR += mask[ xm + 1 ][ ym + 1 ] * curPixel.r;
                    accumG += mask[ xm + 1 ][ ym + 1 ] * curPixel.g;
                    accumB += mask[ xm + 1 ][ ym + 1 ] * curPixel.b;
                    wSum += mask[ xm + 1][ ym + 1 ];
                }
            }

            Pixel blurred = *(new Pixel());
            blurred.r = (accumR / wSum);
            blurred.g = (accumG / wSum);
            blurred.b = (accumB / wSum);
            destination->pixel( x, y ) = *( new Pixel32( blurred ));
        }
    }

    outputImage = *destination;
	return 1;
}

int Image32::EdgeDetect3X3(Image32& outputImage) const
{
    float mask[3][3] = 
    {
        {-1, -1, -1},
        {-1, 8, -1},
        {-1, -1, -1}
    };

    Image32 destination = *( new Image32() );
    destination.setSize( outputImage.width(), outputImage.height() );

    for ( int x = 0; x < outputImage.width(); x++ ) {
        for ( int y = 0; y < outputImage.height(); y++ ) {
            float accumR = 0;
            float accumG = 0;
            float accumB = 0;
            for ( int xm = -1; xm <=1; xm++ ) {
                for ( int ym = -1; ym <= 1; ym++ ) {

                    // Ignore the edges! 
                    if ( x + xm < 0 || 
                            y + ym < 0 || 
                            x + xm == outputImage.width() || 
                            y + ym == outputImage.height() ) {
                        continue;
                    }

                    // Get the sums
                    Pixel curPixel = *( new Pixel( outputImage.pixel( x + xm, y + ym )));
                    accumR += mask[ xm + 1 ][ ym + 1 ] * curPixel.r;
                    accumG += mask[ xm + 1 ][ ym + 1 ] * curPixel.g;
                    accumB += mask[ xm + 1 ][ ym + 1 ] * curPixel.b;
                }
            }

            Pixel edge = *(new Pixel());
            edge.r = accumR;
            edge.g = accumG;
            edge.b = accumB;
            destination.pixel( x, y ) = *( new Pixel32( edge ));
        }
    }

    outputImage = destination;
	return 1;
}
int Image32::ScaleNearest(const float& scaleFactor,Image32& outputImage) const
{
    Image32 dest = *( new Image32() );
    dest.setSize( scaleFactor * outputImage.width(), scaleFactor * outputImage.height() );

    for ( int x = 0; x < dest.width(); x++ ) {
        for ( int y = 0; y < dest.height(); y++ ) {
            float u = x / scaleFactor;
            float v = y / scaleFactor;

            dest.pixel( x, y ) = outputImage.NearestSample( u, v );
        }
    }

    outputImage = dest;
	return 1;
}

int Image32::ScaleBilinear(const float& scaleFactor,Image32& outputImage) const
{
    Image32 dest = *( new Image32() );
    dest.setSize( scaleFactor * outputImage.width(), scaleFactor * outputImage.height() );

    for ( int x = 0; x < dest.width(); x++ ) {
        for ( int y = 0; y < dest.height(); y++ ) {
            float u = (float) x / scaleFactor;
            float v = (float) y / scaleFactor;

            dest.pixel( x, y ) = outputImage.BilinearSample( u, v );
        }
    }

    outputImage = dest;
	return 1;
}

int Image32::ScaleGaussian(const float& scaleFactor,Image32& outputImage) const
{
    Image32 dest = *( new Image32() );
    dest.setSize( scaleFactor * outputImage.width(), scaleFactor * outputImage.height() );

    // 1. Calculate variance
    float sigma = max( 1.0/scaleFactor, 1.0 );
    float variance = pow( sigma, 2.0 ); 
    float n = ceil( sigma * 2.0 );

    // 2. Gaussian Sample
    for ( int x = 0; x < dest.width(); x++ ) {
        for ( int y = 0; y < dest.height(); y++ ) {
            float u = (float) x / scaleFactor;
            float v = (float) y / scaleFactor;

            dest.pixel( x, y ) = outputImage.GaussianSample( u, v, (float) variance, (float) ceil(sigma) );
        }
    }

    outputImage = dest;
	return 1;
}

int Image32::RotateNearest(const float& angle,Image32& outputImage) const
{
    // TODO: Rotations don't rotate "enough". 45 degrees looks like 30, etc.
    // 1. Find what coordinates the image's corners map to
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float maxX = FLT_MIN;
    float maxY = FLT_MIN;

    list<pair < int,int > > corners;
    corners.push_back( pair<int, int>(0, 0));
    corners.push_back( pair<int, int>(0, outputImage.height()));
    corners.push_back( pair<int, int>(outputImage.width(), 0));
    corners.push_back( pair<int, int>(outputImage.width(), outputImage.height()));

    float forwardMappedX; 
    float forwardMappedY;
    for ( list< pair < int,int > >::iterator i = corners.begin(); i != corners.end(); i++ ) {
        int u = i->first;
        int v = i->second;

        forwardMappedX = (float) u * cos(angle) - (float) v * sin(angle);
        forwardMappedY = (float) u * sin(angle) + (float) v * cos(angle);

        minX = min( minX, forwardMappedX );
        minY = min( minY, forwardMappedY );
        maxX = max( maxX, forwardMappedX );
        maxY = max( maxY, forwardMappedY );
    }
    int rotatedWidth = ceil( maxX - minX );
    int rotatedHeight = ceil( maxY - minY );

    Image32 dest = *( new Image32() );
    dest.setSize( rotatedWidth, rotatedHeight );

    // 2. Reverse map to find the pixels
    float xOffset = minX;
    float yOffset = minY;
    float u, v;
    for ( int x = 0; x < dest.width(); x++ ) {
        for ( int y = 0; y < dest.height(); y++ ) {
            u = (x + xOffset) * cos( -angle ) - (y + yOffset) * sin( -angle );
            v = (x + xOffset) * sin( -angle ) + (y + yOffset) * cos( -angle );
            dest.pixel( x, y ) = outputImage.NearestSample( u, v );
        }
    }

    outputImage = dest;

    return 1;
}

int Image32::RotateBilinear(const float& angle,Image32& outputImage) const
{
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float maxX = FLT_MIN;
    float maxY = FLT_MIN;

    list<pair < int,int > > corners;
    corners.push_back( pair<int, int>(0, 0));
    corners.push_back( pair<int, int>(0, outputImage.height()));
    corners.push_back( pair<int, int>(outputImage.width(), 0));
    corners.push_back( pair<int, int>(outputImage.width(), outputImage.height()));

    float forwardMappedX; 
    float forwardMappedY;
    for ( list< pair < int,int > >::iterator i = corners.begin(); i != corners.end(); i++ ) {
        int u = i->first;
        int v = i->second;

        forwardMappedX = (float) u * cos(angle) - (float) v * sin(angle);
        forwardMappedY = (float) u * sin(angle) + (float) v * cos(angle);

        minX = min( minX, forwardMappedX );
        minY = min( minY, forwardMappedY );
        maxX = max( maxX, forwardMappedX );
        maxY = max( maxY, forwardMappedY );
    }
    int rotatedWidth = ceil( maxX - minX );
    int rotatedHeight = ceil( maxY - minY );

    Image32 dest = *( new Image32() );
    dest.setSize( rotatedWidth, rotatedHeight );

    // 2. Reverse map to find the pixels
    float xOffset = minX;
    float yOffset = minY;
    float u, v;
    for ( int x = 0; x < dest.width(); x++ ) {
        for ( int y = 0; y < dest.height(); y++ ) {
            u = (x + xOffset) * cos( -angle ) - (y + yOffset) * sin( -angle );
            v = (x + xOffset) * sin( -angle ) + (y + yOffset) * cos( -angle );
            dest.pixel( x, y ) = outputImage.BilinearSample( u, v );
        }
    }

    outputImage = dest;

	return 1;
}
	
int Image32::RotateGaussian(const float& angle,Image32& outputImage) const
{
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float maxX = FLT_MIN;
    float maxY = FLT_MIN;

    list<pair < int,int > > corners;
    corners.push_back( pair<int, int>(0, 0));
    corners.push_back( pair<int, int>(0, outputImage.height()));
    corners.push_back( pair<int, int>(outputImage.width(), 0));
    corners.push_back( pair<int, int>(outputImage.width(), outputImage.height()));

    float forwardMappedX; 
    float forwardMappedY;
    for ( list< pair < int,int > >::iterator i = corners.begin(); i != corners.end(); i++ ) {
        int u = i->first;
        int v = i->second;

        forwardMappedX = (float) u * cos(angle) - (float) v * sin(angle);
        forwardMappedY = (float) u * sin(angle) + (float) v * cos(angle);

        minX = min( minX, forwardMappedX );
        minY = min( minY, forwardMappedY );
        maxX = max( maxX, forwardMappedX );
        maxY = max( maxY, forwardMappedY );
    }
    int rotatedWidth = ceil( maxX - minX );
    int rotatedHeight = ceil( maxY - minY );

    Image32 dest = *( new Image32() );
    dest.setSize( rotatedWidth, rotatedHeight );

    // 2. Reverse map to find the pixels
    float sigma = 1.0;
    float variance = pow( sigma, 2.0 ); 

    float xOffset = minX;
    float yOffset = minY;
    float u, v;
    for ( int x = 0; x < dest.width(); x++ ) {
        for ( int y = 0; y < dest.height(); y++ ) {
            u = (x + xOffset) * cos( -angle ) - (y + yOffset) * sin( -angle );
            v = (x + xOffset) * sin( -angle ) + (y + yOffset) * cos( -angle );
            dest.pixel( x, y ) = outputImage.GaussianSample( u, v, (float) variance, (float) ceil(sigma) );
        }
    }

    outputImage = dest;
	return 1;
}


// TODO: how?
int Image32::SetAlpha(const Image32& matte)
{
    for ( int x = 0; x < this->width(); x++ ) {
        for ( int y = 0; y < this->height(); y++ ) {
            if ( x >= matte.width() || y >= matte.height() ) {
                continue;
            }

            Pixel curPixel = *( new Pixel( this->pixel(x, y) ) );
            Pixel mattePixel = *( new Pixel( matte.pixel(x, y) ) );
            float aAlpha = curPixel.a;
            float bAlpha = mattePixel.a;

            curPixel.r = aAlpha * curPixel.r + ( 1 - aAlpha ) * bAlpha * mattePixel.r;
            curPixel.g = aAlpha * curPixel.g + ( 1 - aAlpha ) * bAlpha * mattePixel.g;
            curPixel.b = aAlpha * curPixel.b + ( 1 - aAlpha ) * bAlpha * mattePixel.b;

            this->pixel(x, y) = *( new Pixel32( curPixel ) );
        }
    }

	return 0;
}

int Image32::Composite(const Image32& overlay,Image32& outputImage) const
{
    outputImage.SetAlpha( overlay );
    // outputImage.SetAlpha( *this );
	return 0;
}

int Image32::CrossDissolve(const Image32& source,const Image32& destination,const float& blendWeight,Image32& ouputImage)
{
	return 0;
}
int Image32::Warp(const OrientedLineSegmentPairs& olsp,Image32& outputImage) const
{
	return 0;
}

int Image32::FunFilter(Image32& outputImage) const
{
	return 0;
}

int Image32::Crop(const int& x1,const int& y1,const int& x2,const int& y2,Image32& outputImage) const
{
    Image32 *croppedImage = new Image32();
    if( croppedImage->setSize( (x2-x1), (y2-y1)) ) {
        int i = 0;
        int j = 0;
        for ( int x = x1; x < x2; x++ ) {
            for ( int y = y1; y < y2; y++  ) {
                croppedImage->pixel( i, j ) = outputImage.pixel( x, y );
                j++;
            }
            j = 0;
            i++;
        }

        outputImage = *croppedImage;

        return 1;
    } else {
        return 0;
    }
}

Pixel32 Image32::NearestSample(const float& x,const float& y) const
{
    // n +- 0.5 will round to n
    int iu = (int) ( x + 0.5 );
    int iv = (int) ( y + 0.5 );

    if ( iu < 0 || iu >= w || iv < 0 || iv >= h ) {
        Pixel32 blackPixel = *( new Pixel32() );
        blackPixel.r = 0;
        blackPixel.g = 0;
        blackPixel.b = 0;
        return blackPixel;
    }

    return pixels[iu + iv*w];
}

Pixel32 Image32::BilinearSample(const float& x,const float& y) const
{
    int x1 = floor( x );
    int x2 = x1 + 1;
    int y1 = floor( y );
    int y2 = y1 + 1;
    float dx = ( (float) x - (float) x1 );
    float dy = ( (float) y - (float) y1 );

    if ( x1 < 0 || x2 >= w || y1 < 0 || y2 >= h ) {
        return NearestSample( x, y );
    }

    // 1. Interpolate x-values
    float aR = pixels[x1 + y1*w].r * ( 1.0 - dx ) + pixels[x2 + y1*w].r * dx;  
    float aG = pixels[x1 + y1*w].g * ( 1.0 - dx ) + pixels[x2 + y1*w].g * dx;  
    float aB = pixels[x1 + y1*w].b * ( 1.0 - dx ) + pixels[x2 + y1*w].b * dx;  

    float bR = pixels[x1 + y2*w].r * ( 1.0 - dx ) + pixels[x2 + y2*w].r * dx;  
    float bG = pixels[x1 + y2*w].g * ( 1.0 - dx ) + pixels[x2 + y2*w].g * dx;  
    float bB = pixels[x1 + y2*w].b * ( 1.0 - dx ) + pixels[x2 + y2*w].b * dx;  

    // 2. Interpolate y-values
    float finalR = aR * (1.0 - dy) + bR * dy;
    float finalG = aG * (1.0 - dy) + bG * dy;
    float finalB = aB * (1.0 - dy) + bB * dy;

    Pixel final = *( new Pixel() );
    final.r = ( finalR / 255.0 );
    final.g = ( finalG / 255.0 );
    final.b = ( finalB / 255.0 );

	return *( new Pixel32( final ) );
}

Pixel32 Image32::GaussianSample(const float& x,const float& y,const float& variance,const float& radius) const
{
    float e = exp( 1.0 );

    float sigma = max( variance, (float) 1.0 );
    float exponent;

    float xDelta;
    float yDelta;

    float curWeight;
    float accumR = 0.0;
    float accumG = 0.0;
    float accumB = 0.0;
    float total = 0.0;

    if ( x < 0 || x >= w || y < 0 || y >= h ) {
        Pixel32 blackPixel = *( new Pixel32() );
        blackPixel.r = 0;
        blackPixel.g = 0;
        blackPixel.b = 0;
        return blackPixel;
    }

    for ( int i = x - radius; i <= x + radius; i++ ) {
        for ( int j = y - radius; j <= y + radius; j++ ) {
            if ( i < 0 || i >= w || j < 0 || j >= h ) {
                continue;
            }

            int xNN = (int) ( i + 0.5 );
            int yNN = (int) ( j + 0.5 );

            xDelta = (float) xNN - (float) x;
            yDelta = (float) yNN - (float) y;

            exponent = ( pow( xDelta, 2.0 ) + pow ( yDelta, 2.0 ) ) / ( 2.0 * pow( sigma, 2.0 ));
            curWeight = pow( e, (float) -( exponent ) );   

            Pixel curPixel = *( new Pixel( pixels[xNN + yNN*w] ));
            accumR += curPixel.r * curWeight;
            accumG += curPixel.g * curWeight;
            accumB += curPixel.b * curWeight;
            total += curWeight;
        }
    }

    Pixel gaussianPixel = *(new Pixel());
    gaussianPixel.r = accumR / total;
    gaussianPixel.g = accumG / total;
    gaussianPixel.b = accumB / total;

	return *( new Pixel32( gaussianPixel ) );
}
