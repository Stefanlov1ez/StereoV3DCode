/* -*-c++-*- StereoV3D - Copyright (C) 2021.
* Author	: Ethan Li<ethan.li.whu@gmail.com>
* https://github.com/ethan-li-coding/StereoV3DCode
*/

#ifndef SV3D_MATRIX_ESSENTIAL_H
#define SV3D_MATRIX_ESSENTIAL_H

#include "../eigen_defs.h"

namespace  sv3d
{

	class EssentialSolver
	{
	public:
		EssentialSolver() = default;
		~EssentialSolver() = default;
		
		enum SOLVE_TYPE {
			EIGHT_POINTS = 0,
			//FIVE_POINTS
		};

		/**
		 * \brief ���ʾ������
		 * \param p1 ��ͼ1�����ص��������
		 * \param p2 ��ͼ2�����ص��������
		 * \param k1_mat ���1�ڲξ���
		 * \param k2_mat ���2�ڲξ���
		 * \param solver_type ����㷨����
		 */
		void Solve(const Mat3X p1, const Mat3X p2, const RMat3 k1_mat, const RMat3 k2_mat, const SOLVE_TYPE& solver_type);

		/**
		 * \brief ���ʾ������
		 * \param x1 ��ͼ1�ϵĹ�һ�����ص��������(x = k_inv*p)
		 * \param x2 ��ͼ2�ϵĹ�һ�����ص��������
		 * \param solver_type ����㷨����
		 */
		void Solve(const Mat3X x1, const Mat3X x2, const SOLVE_TYPE& solver_type);

		/**
		 * \brief ��ȡ���ʾ���
		 * \return ���ʾ���
		 */
		Mat3 Value();
	private:

		/**
		 * \brief �˵㷨��Ȿ�ʾ���
		 * \param x1 ��ͼ1�ϵĹ�һ�����ص��������(x = k_inv*p)
		 * \param x2 ��ͼ2�ϵĹ�һ�����ص��������
		 */
		void Solve_EightPoints(const Mat3X x1, const Mat3X x2);

		//void Solve_FivePoints(const Mat3X x1, const Mat3X x2);

		/* ���ʾ������� */
		Mat3 data_;
	};

}

#endif