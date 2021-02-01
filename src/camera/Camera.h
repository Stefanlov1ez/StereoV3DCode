/* -*-c++-*- StereoV3D - Copyright (C) 2021.
* Author	: Ethan Li<ethan.li.whu@gmail.com>
* https://github.com/ethan-li-coding/StereoV3DCode
* Describe	: header of camera class
*/

#pragma once
#include <Eigen/Dense>

using namespace Eigen;

// �����ģ��ͨ����ʽ����
//   P = K[R,t]
// ����R��t��ʾ����������������ϵ����̬��ƽ��
// R Ϊ�������ϵ�������������ϵ����ת����
// t Ϊ�������ϵ�������������ϵ��ƽ��ʸ��
// �������ϵ����������ϵ��Ϊ����ϵ
typedef Matrix<double, 3, 3, Eigen::RowMajor> Matrix3dr;
typedef Matrix<double, 3, 4, Eigen::RowMajor> Matrix34dr;
class Camera {
private:
	Matrix3dr	K_;			// �ڲξ���
    Matrix3dr	R_;			// ��ת����
	Vector3d	t_;			// ƽ�ƾ���
	Matrix34dr	P_;			// ͶӰ����
	Matrix3dr	KI_;		// �ڲξ���������
	Matrix3dr	RT_;		// ��ת����������
	
public:
    inline Camera() = default;
	inline ~Camera() = default;
	
	/** ͨ��K��R��t�����������ע�����������ƶ�������⸴�Ʋ���������Ĳ�����ִ�й��캯�����ڴ����٣�����ʹ����ʱ�������� */
	inline Camera(Matrix3dr K, Matrix3dr R, Vector3d t) : K_(std::move(K)), R_(std::move(R)), t_(std::move(t)) {
		KI_ = K_.inverse();
		RT_ = R_.transpose();
		P_ << K_*R_, K_*t_;
	}
   
	/*****************************************����������ϵת���ӿ�*****************************************/
	/** �������ϵת������������ϵ */
	inline Vector3d TransformPointC2W(const Vector3d& X) const {
		return R_.transpose() * (X - t_);
	}
	/** ��������ϵ���������ϵ */
	inline Vector3d TransformPointW2C(const Vector3d& X) const {
		return R_*X + t_;
	}
	/** �������ϵ��Ӱ������ϵ */
	inline Vector2d TransformPointC2I(const Vector3d& X) const {
		auto I = Vector3d(K_*X);
		return Vector2d(I(0) / I(2), I(1) / I(2));
	}
	/** ��������ϵ��Ӱ������ϵ */
	inline Vector2d TransformPointW2I(const Vector3d& X) const {
		return TransformPointC2I(TransformPointW2C(X));
	}
	/**  Ӱ������ϵת�����������ϵ(�����֪��X[2]) */
	inline Vector3d TransformPointI2C(const Vector3d& X) const {
		auto Xt = X;
		Xt[0] *= Xt[2]; Xt[1] *= Xt[2];
		return KI_*Xt;
	}
	/**  Ӱ������ϵת������������ϵ(�����֪��X[2]) */
	inline Vector3d TransformPointI2W(const Vector3d& X) const {
		return TransformPointC2W(TransformPointI2C(X));
	}
	/*****************************************����������ϵת���ӿ�*****************************************/
	
};