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
 * @ingroup math geometry
 * @file
 * Functions to project a 3D point into 2D space.
 *
 * @author Christian Waechter <christian.waechter@in.tum.de>
 */ 

 
 
#ifndef __H__POINT_PROJECTION_FUNCTIONS__
#define __H__POINT_PROJECTION_FUNCTIONS__


#include <utMath/Vector.h> //includes static assert
#include <utMath/Matrix.h>

#include "container_traits.h"
#include "../Util/type_traits.h"
#include "../Stochastic/identity_iterator.h"

#include <algorithm> //std::transform

namespace Ubitrack { namespace Math { namespace Geometry {


/**
 * @ingroup math geometry functor
 * @brief Functor to project a \b 3D \b point into \b 2D \b space.
 *
 * This functors provides several overloaded bracket \c operator() for
 * different types of \b 3D \b point representation.
 *
 * Possible \b 3D \b point representations:
 * - \b Vector2 (homogeneous point, 3rd dimension is assumed as 0 )
 * - \b Vector3 (common representation)
 * - \b Vector4 (e.g. homogeneous representation with one for last dimension, but can also be different )
 */
struct ProjectPoint
{

public:
	// internal template to catch wrong vector types and print an error message
	template< typename notSupportedMatrixType, typename notSupportedVectorType >
	notSupportedVectorType operator() ( const notSupportedMatrixType&, const notSupportedVectorType& vec_in ) const
	{
		UBITRACK_STATIC_ASSERT( false, ONLY_VECTORS_OF_2_3_OR_4_DIMENSIONS_SUPPORTED );
		return vec_in;
	}
	
	
	/// @internal Specialization of \c bracket-operator for projection of \b 2D \b points
	template< typename T >
	Math::Vector< T, 2 > operator() ( const Math::Matrix< T, 3, 3 > &projMat, const Math::Vector< T, 2 > &vec ) const
	{
		const T e1 = projMat( 0, 0 ) * vec( 0 ) + projMat( 0, 1 ) * vec( 1 ) + projMat( 0, 2 );
		const T e2 = projMat( 1, 0 ) * vec( 0 ) + projMat( 1, 1 ) * vec( 1 ) + projMat( 1, 2 );
		const T e3 = projMat( 2, 0 ) * vec( 0 ) + projMat( 2, 1 ) * vec( 1 ) + projMat( 2, 2 );
		return Math::Vector< T, 2 > ( e1/e3, e2/e3 );
	}
	
	
	/// @internal Specialization of \c bracket-operator for projection of \b 2D \b points
	template< typename T >
	Math::Vector< T, 2 > operator() ( const Math::Matrix< T, 3, 4 > &projMat, const Math::Vector< T, 2 > &vec ) const
	{
		// const T e1 = projMat( 0, 0 ) * vec( 0 ) + projMat( 0, 1 ) * vec( 1 ) + projMat( 0, 2 );
		// const T e2 = projMat( 1, 0 ) * vec( 0 ) + projMat( 1, 1 ) * vec( 1 ) + projMat( 1, 2 );
		// const T e3 = projMat( 2, 0 ) * vec( 0 ) + projMat( 2, 1 ) * vec( 1 ) + projMat( 2, 2 );
		// assume the 3rd position to be set to zero, therefore we skip 3 column of projMat
		const T e1 = projMat( 0, 0 ) * vec( 0 ) + projMat( 0, 1 ) * vec( 1 ) + projMat( 0, 3 );
		const T e2 = projMat( 1, 0 ) * vec( 0 ) + projMat( 1, 1 ) * vec( 1 ) + projMat( 1, 3 );
		const T e3 = projMat( 2, 0 ) * vec( 0 ) + projMat( 2, 1 ) * vec( 1 ) + projMat( 2, 3 );
		return Math::Vector< T, 2 > ( e1/e3, e2/e3 );
	}
	
	/// @internal Specialization of \c bracket-operator for projection of \b 3D \b points 
	template< typename T >
	Math::Vector< T, 2 > operator() ( const Math::Matrix< T, 3, 4 > &projMat, const Math::Vector< T, 3 > &vec ) const
	{
		const T e1 = projMat( 0, 0 ) * vec( 0 ) + projMat( 0, 1 ) * vec( 1 ) + projMat( 0, 2 ) * vec( 2 ) + projMat( 0, 3 );
		const T e2 = projMat( 1, 0 ) * vec( 0 ) + projMat( 1, 1 ) * vec( 1 ) + projMat( 1, 2 ) * vec( 2 ) + projMat( 1, 3 );
		const T e3 = projMat( 2, 0 ) * vec( 0 ) + projMat( 2, 1 ) * vec( 1 ) + projMat( 2, 2 ) * vec( 2 ) + projMat( 2, 3 );
		return Math::Vector< T, 2 > ( e1/e3, e2/e3 );
	}
	
	/// @internal Specialization of \c bracket-operator for projection of \b 4D \b points
	template< typename T >
	Math::Vector< T, 2 > operator() ( const Math::Matrix< T, 3, 4 > &projMat, const Math::Vector< T, 4 > &vec ) const
	{
		const T e1 = projMat( 0, 0 ) * vec( 0 ) + projMat( 0, 1 ) * vec( 1 ) + projMat( 0, 2 ) * vec( 2 ) + projMat( 0, 3 ) * vec( 3 );
		const T e2 = projMat( 1, 0 ) * vec( 0 ) + projMat( 1, 1 ) * vec( 1 ) + projMat( 1, 2 ) * vec( 2 ) + projMat( 1, 3 ) * vec( 3 );
		const T e3 = projMat( 2, 0 ) * vec( 0 ) + projMat( 2, 1 ) * vec( 1 ) + projMat( 2, 2 ) * vec( 2 ) + projMat( 2, 3 ) * vec( 3 );
		return Math::Vector< T, 2 > ( e1/e3, e2/e3 );
	}
	
	
	/// @internal Specialization of \c bracket-operator to flip the arguments if coming in inverse order
	template< typename T, size_t L, size_t M, size_t N >
	Math::Vector< T, 2 > operator() ( const Math::Vector< T, L > & lhs, const Math::Matrix< T, M, N > &rhs ) const
	{
		return operator()( rhs, lhs );
	}
	
	/* // not really needed, see the flip function above
	struct FlipArguements
	{
		template< typename VectorType, typename MatrixType >
		Math::Vector< T, 2 > operator() ( const VectorType& lhs, const MatrixType& rhs ) const
		{
			return ProjectPoint()( rhs, lhs );
		}
	};
	*/
};


/**
 * @ingroup math geometry
 * @brief Projects several points using iterators pointing to the storage class of the points.
 *
 * This function can be applied to nearly any storage class providing 
 * access to the single points via forward iterators
 * (e.g. \c std::vector, \c std::list or \c std::set, etc ).
 * Although designed for stl-containers it is not limited to these ones
 * and this function can be applied to fixed size arrays as well or other storage classes.
 *
 * The function can project 3D points in either \b 2D, \b 3D or \b 4D \b representation and therefore assumes homogeneous 
 * coordinates for the lower dimensional cases ( \b 2D and \b 3D ).
 * It can perform the following actions:
 * - \b 2D : @f$ \hat{p}_{3x1} = P_{3x4} \cdot [p_{1} p_{2} 0 1]^T @f$
 * - \b 3D : @f$ \hat{p}_{3x1} = P_{3x4} \cdot [p_{1} p_{2} p_{3} 1]^T @f$ 
 * - \b 4D : @f$ \hat{p}_{3x1} = P_{3x4} \cdot [p_{1} p_{2} p_{3} p_{4}]^T @f$
 * \n and finally projects the points via @f$ [\hat{p_{1}} \hat{p_{2}}]^T / \hat{p_{3}} @f$
 * 
 * Example use case:\n
 @code
 Matrix< double, 3, 4 > proj; // <- should be filled with values
 std::vector< Vector3d > points3d; // <- should be filled with values
 std::vector< Vector2d > points2d; // <- will be filled with values
 project_points( proj, points3d.begin(), points3d.end(), points2d.begin() );
 // or
 poins2d.reserve( n ); // <- storage allocation via reserve() and number of elements( =n )
 project_points( proj, points3d.begin(), points3d.end(), std::back_inserter( points2d ) );
 @endcode
 * 
 * @tparam T built-in type of matrix and input/output vectors ( e.g. \c double or \c float )
 * @tparam M first dimension of matrix (rows)
 * @tparam N second dimension of matrix (columns)
 * @tparam ForwardIterator1 type of forward iterator to container of input points
 * @tparam ForwardIterator2 type of the output iterator for the projected points
 * @param projection the \b 3-by-4 \b projection \b matrix applied to project the \b points
 * @param iBegin \c iterator pointing to first element in the input container/storage class of the \b points ( usually \c begin() )
 * @param iEnd \c iterator pointing behind the last element in the input container/storage class of the \b points ( usually \c end() )
 * @param iOut output \c iterator pointing to first element in container/storage class for storing the projected points as a result ( usually \c begin() or \c std::back_inserter(container) )
 */
template< typename T, std::size_t M, std::size_t N, typename ForwardIterator1, typename ForwardIterator2 >
inline void project_points( const Math::Matrix< T, M, N > &projection, const ForwardIterator1 iBegin, const ForwardIterator1 iEnd, ForwardIterator2 iOut )
{

	// determine the types of the iterators.
	// Since output iterators (e.g. std::back_insert_itertator via std::back_inserter)
	// can be used as well a simple value_type is not enough
	// -> therefore designed an own container_traits struct
	typedef typename Ubitrack::Util::container_traits< ForwardIterator1 >::value_type vector_type_in;
	typedef typename Ubitrack::Util::container_traits< ForwardIterator2 >::value_type vector_type_out;
	typedef typename vector_type_in::value_type value_type_in;
	typedef typename vector_type_out::value_type value_type_out;

	UBITRACK_STATIC_ASSERT( ( (M == 3) && ((N == 4)  || (N == 3) )) , EXPECTS_3_BY_4_OR_3_BY_3_PROJECTION_MATRIX );
	UBITRACK_STATIC_ASSERT( ( Ubitrack::Util::is_same< value_type_in, T >::value ), MATRIX_AND_VECTORS_NEED_SAME_BUILTIN_TYPE ); // e.g. only float or only double
	UBITRACK_STATIC_ASSERT( ( Ubitrack::Util::is_same< value_type_in, value_type_out >::value ), INPUT_AND_OUTPUT_VECTOR_NEED_SAME_BUILTIN_TYPE );
	UBITRACK_STATIC_ASSERT( ( Ubitrack::Util::is_same< vector_type_out, Math::Vector< T, 2 > >::value ), OUTPUT_VECTOR_NEEDS_TO_BE_DEFINED_WITH_2_DIMENSIONS );

	Ubitrack::Util::identity< const Math::Matrix< T, M, N > > id_container( projection );
	std::transform( iBegin, iEnd, id_container.begin(), iOut, ProjectPoint() );
	
	//const std::size_t n = std::distance( iBegin, iEnd );
	//Ubitrack::Util::identity< const Math::Matrix< T, M, N > > id_container( projection, n );
	//std::transform( id_container.begin(), id_container.end(), iBegin, iOut, ProjectPoint() );
	
	// std::transform( iBegin, iEnd, iOut, std::bind1st( ProjectPoint< T, M, N, vector_type_in >(), projection ) );
}


} } } // namespace Ubitrack::Math::Geometry

#endif //__H__POINT_PROJECTION_FUNCTIONS__
