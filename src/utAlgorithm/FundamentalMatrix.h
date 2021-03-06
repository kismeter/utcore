/*
 * Ubitrack - Library for Ubiquitous Tracking
 * Copyright 2006, Technische Universitaet Muenchen, and individual
 * contributors as indicated by the @authors tag. See the 
 * copyright.txt in the distribution for a full listing of individual
 * contributors.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

/**
 * @ingroup tracking_algorithms
 * @file
 * Functions for fundamental matrix estimation.
 *
 * @author Daniel Muhra <muhra@in.tum.de>
 */ 

#ifndef __UBITRACK_CALIBRATION_FUNDAMENTALMATRIX_H_INCLUDED__
#define __UBITRACK_CALIBRATION_FUNDAMENTALMATRIX_H_INCLUDED__

#include <utCore.h>
#include <utMath/Vector.h>
#include <utMath/Matrix.h>

#ifdef HAVE_LAPACK

namespace Ubitrack { namespace Algorithm {

/**
 * @ingroup tracking_algorithms
 * Computes a fundamental matrix using the normalized 8-point algorithm.
 *
 * The result is a Matrix F that maps points x to points x' via x'Hx=0.
 * See Hartley&Zisserman for details.
 *
 * Note: also exists with \c double parameters.
 *
 * @param fromPoints Points x as inhomogeneous 2-vectors
 * @param toPoints Points x' as inhomogeneous 2-vectors
 * @param stepSize optional parameter, if you want to use e.g. only every second value for computation, then set stepSize to 2
 * @return calculated fundamental matrix
 */
UBITRACK_EXPORT Math::Matrix< float, 3, 3 > getFundamentalMatrix( const std::vector< Math::Vector< float, 2 > >& fromPoints, 
	const std::vector< Math::Vector< float, 2 > >& toPoints, std::size_t stepSize = 1 );

UBITRACK_EXPORT Math::Matrix< double, 3, 3 > getFundamentalMatrix( const std::vector< Math::Vector< double, 2 > >& fromPoints, 
	const std::vector< Math::Vector< double, 2 > >& toPoints, std::size_t stepSize = 1 );

/**
 * @ingroup tracking_algorithms
 * Computes a fundamental matrix from two camera poses
 *
 * The result is a Matrix F that maps points x to points x' via x'Hx=0.
 * See Hartley&Zisserman for details.
 *
 * @param cam1 pose of the first camera
 * @param cam2 pose of the second camera
 * @param K1 intrinsic matrix of the first camera
 * @param K2 intrinsic matrix of second first camera
 * @return calculated fundamental matrix
 */
UBITRACK_EXPORT Math::Matrix< double, 3, 3 > fundamentalMatrixFromPoses( const Math::Pose & cam1, const Math::Pose & cam2, const Math::Matrix< double, 3, 3 > & K1, const Math::Matrix< double, 3, 3 > & K2 );

/**
 * @ingroup tracking_algorithms
 * Computes the pose of a second camera relative to the first camera
 *
 * The result is a Math Pose
 *
 * @param F the fundamental matrix
 * @param x the 2D coordinates of a point seen by the first camera
 * @param x_ the 2D coordinates of a point seen by the second camera
 * @param K1 intrinsic matrix of the first camera
 * @param K2 intrinsic matrix of second first camera
 * @return estimated position of the second camera
 */
UBITRACK_EXPORT Math::Pose poseFromFundamentalMatrix( const Math::Matrix< double, 3, 3 > & fM, const Math::Vector< double, 2 > & x, const Math::Vector< double, 2 > & x_, const Math::Matrix< double, 3, 3 > & K1, const Math::Matrix< double, 3, 3 > & K2 );

/**
 * function object version of getFundamentalMatrix for RANSAC etc.
 */
template< class T >
class GetFundamentalMatrix
{
public:
	void operator()( Math::Matrix< T, 3, 3 >& result, const std::vector< Math::Vector< T, 2 > >& fromPoints, 
		const std::vector< Math::Vector< T, 2 > >& toPoints ) const
	{ result = getFundamentalMatrix( fromPoints, toPoints ); }
};

/**
 * function object to evalute a fundamental matrix for RANSAC etc.
 */
template< class T >
class EvaluateFundamentalMatrix
{
public:
	/**
	 * computes squared distance of "to" point to epipolar line determined by f * from
	 */
	T operator()( const Math::Matrix< T, 3, 3 >& fM, const Math::Vector< T, 2 >& from, const Math::Vector< T, 2 >& to ) const
	{
		Math::Vector< T, 3 > from_;
		for ( unsigned i = 0; i < 3; i++ )
			from_( i ) = fM( i, 0 ) * from( 0 ) + fM( i, 1 ) * from( 1 ) + fM( i, 2 );

		T term = from_( 0 ) * to( 0 ) + from_( 1 )* to( 1 ) + from_( 2 );
		return ( term * term ) / ( from_( 0 ) * from_( 0 ) + from_( 1 )* from_( 1 ) );
	}
};
	
} } // namespace Ubitrack::Algorithm

#endif // HAVE_LAPACK

#endif
