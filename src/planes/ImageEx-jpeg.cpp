/*
	This file is part of Overmix.

	Overmix is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Overmix is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Overmix.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ImageEx.hpp"

#include "../color.hpp"
#include "../debug.hpp"

#include "../degraders/JpegDegrader.hpp"

#include "gwenview/iodevicejpegsourcemanager.h"
#include "jpeglib.h"

#include <QFile>

#include <vector>
//#include <QImage>

class ImgRow{
	private:
		std::vector<color_type*> rows;
		unsigned width;
		
	public:
		ImgRow( ImageEx& img, unsigned iy, int colors )
			:	width( img.get_width() ){
			rows.reserve( colors );
			for( int i=0; i<colors; i++ )
				rows.push_back( img[i].scan_line( iy ) );
		}
		
		void read8( unsigned char* row_pointer ){
			for( unsigned ix=0; ix<width; ix++ )
				for( unsigned i=0; i<rows.size(); i++ )
					rows[i][ix] = color::from8bit( row_pointer[ix*rows.size() + i] );
		}
};

JpegDegrader ImageEx::getJpegDegrader( QString path ){
	JpegDegrader deg;
	
	QFile f( path );
	if( !f.open( QIODevice::ReadOnly ) )
		return deg;
	
	ImageEx().from_jpeg( f, &deg );
	return deg;
}

bool ImageEx::from_jpeg( QIODevice& dev, JpegDegrader* deg ){
	Timer t( "from_jpeg" );
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	
	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_decompress( &cinfo );
	
	Gwenview::IODeviceJpegSourceManager::setup( &cinfo, &dev );
	jpeg_read_header( &cinfo, true );
	
	/*
	auto v_ptr = jpeg_read_coefficients( &cinfo );
	auto blockarr = cinfo.mem->access_virt_barray( (j_common_ptr)&cinfo, v_ptr[0], 0, 1, false );
	for( unsigned j=0; j<5; j++ ){
		auto block = blockarr[0][j];
		QString out;
		for( unsigned i=0; i<64; i++ )
			out += QString::number( block[i] ) + " ";
		qDebug() << out;
	}
	/*/
	jpeg_start_decompress( &cinfo );
	
	Size<unsigned> size( cinfo.output_width, cinfo.output_height );
	switch( cinfo.out_color_components ){ //jpeg_color_space, JCS_YCbCr GRAYSCALE, RGB
		case 1: type = GRAY; qCDebug(LogImageIO) << "Gray-scale JPEG"; break;
		case 3: type = RGB; break;
		default: qCWarning(LogImageIO) << "Unknown components count:" << cinfo.out_color_components;
	}
	
	
	for( int i=0; i<cinfo.output_components; i++ )
		planes.emplace_back( size );
	
	std::vector<unsigned char> buffer( cinfo.output_components*size.width() );
	for( unsigned iy=0; iy<size.height(); iy++ ){
		auto arr = buffer.data(); //TODO: Why does it need the address of the pointer?
		jpeg_read_scanlines( &cinfo, &arr, 1 );
		
		ImgRow( *this, iy, cinfo.output_components ).read8( arr );
	}
	
	if( deg ){
		*deg = JpegDegrader();
		//TODO: set color type
		
		//Find the maximum sampling factor
		int max_h = 1, max_v = 1;
		for( int i=0; i<cinfo.output_components; i++ ){
			max_h = std::max( max_h, cinfo.comp_info[i].h_samp_factor );
			max_v = std::max( max_v, cinfo.comp_info[i].v_samp_factor );
		}
		
		for( int i=0; i<cinfo.output_components; i++ )
			deg->addPlane( { {cinfo.comp_info[i].quant_table->quantval}
					,	max_h / double(cinfo.comp_info[i].h_samp_factor)
					,	max_v / double(cinfo.comp_info[i].v_samp_factor)
				} );
	}
	
	jpeg_finish_decompress( &cinfo );//*/
	jpeg_destroy_decompress( &cinfo );
	
	return true;
}