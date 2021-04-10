/* -*-c++-*- StereoV3D - Copyright (C) 2021.
* Author	: Ethan Li<ethan.li.whu@gmail.com>
* https://github.com/ethan-li-coding/StereoV3DCode
*/

#include "datasets.h"
#include <random>

void sv3d::SimulativeStereoDataset::GenarateHomonymyPairs(const unsigned& k, Mat3X& p1, Mat3X& p2)
{
	// ����������Ŀռ����������k���ռ��
	const double kDepthMin = 300., kDepthMax = 500.;

	// ����С���ƽ������ĸ��ǵ���������꣨��������ϵ=���������ϵ��
	Vec3 x1(0, 0, kDepthMin), x2(0, h-1, kDepthMin),
		x3(w-1, 0, kDepthMin), x4(w - 1, h - 1, kDepthMin);
	auto P1 = cam1.TransformPointI2W(x1);
	auto P2 = cam1.TransformPointI2W(x2);
	auto P3 = cam1.TransformPointI2W(x3);
	auto P4 = cam1.TransformPointI2W(x4);

	// ����һ���ռ������壬�������巶Χ��������ɿռ��
	const auto z_min(kDepthMin), z_max(kDepthMax);
	const auto x_min = (std::min)(P1.data()[0], P2.data()[0]);
	const auto x_max = (std::max)(P3.data()[0], P4.data()[0]);
	const auto y_min = (std::min)(P1.data()[0], P3.data()[0]);
	const auto y_max = (std::max)(P2.data()[0], P4.data()[0]);

	// �������k���ռ��
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> rand(0.0f, 1.0f);

	const auto x_range = x_max - x_min;
	const auto y_range = y_max - y_min;
	const auto z_range = z_max - z_min;
	p1.resize(3, k);
	p2.resize(3, k);
	for (unsigned n = 0; n < k; n++) {
		Vec3 w;
		w[0] = rand(gen) * x_range + x_min;
		w[1] = rand(gen) * y_range + y_min;
		w[2] = rand(gen) * z_range + z_min;

		Vec2 x1 = cam1.TransformPointW2I(w);
		Vec2 x2 = cam2.TransformPointW2I(w);

		p1.data()[3 * n] = x1[0]; p1.data()[3 * n + 1] = x1[1]; p1.data()[3 * n + 2] = 1.;
		p2.data()[3 * n] = x2[0]; p2.data()[3 * n + 1] = x2[1]; p2.data()[3 * n + 2] = 1.;
	}
}

void sv3d::SimulativeStereoDataset::GenarateHomonymyPairsInPlane(const unsigned& k, Mat3X& p1, Mat3X& p2, Mat3& H)
{
	// �����������ĳ���ƽ�����������k���ռ��
	const double kDepth = 400.;

	// �����ƽ������ĸ��ǵ���������꣨��������ϵ=���������ϵ��
	Vec3 x1(0, 0, kDepth), x2(0, h - 1, kDepth),
		x3(w - 1, 0, kDepth), x4(w - 1, h - 1, kDepth);
	auto P1 = cam1.TransformPointI2W(x1);
	auto P2 = cam1.TransformPointI2W(x2);
	auto P3 = cam1.TransformPointI2W(x3);
	auto P4 = cam1.TransformPointI2W(x4);

	// ����һ���ռ���棬���淶Χ��������ɿռ��
	const auto z(kDepth);
	const auto x_min = (std::min)(P1.data()[0], P2.data()[0]);
	const auto x_max = (std::max)(P3.data()[0], P4.data()[0]);
	const auto y_min = (std::min)(P1.data()[0], P3.data()[0]);
	const auto y_max = (std::max)(P2.data()[0], P4.data()[0]);

	// �������k���ռ��
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> rand(0.0f, 1.0f);

	const auto x_range = x_max - x_min;
	const auto y_range = y_max - y_min;
	p1.resize(3, k);
	p2.resize(3, k);
	for (unsigned n = 0; n < k; n++) {
		Vec3 w;
		w[0] = rand(gen) * x_range + x_min;
		w[1] = rand(gen) * y_range + y_min;
		w[2] = z;

		Vec2 x1 = cam1.TransformPointW2I(w);
		Vec2 x2 = cam2.TransformPointW2I(w);

		p1.data()[3 * n] = x1[0]; p1.data()[3 * n + 1] = x1[1]; p1.data()[3 * n + 2] = 1.;
		p2.data()[3 * n] = x2[0]; p2.data()[3 * n + 1] = x2[1]; p2.data()[3 * n + 2] = 1.;
	}

	// ���㵥Ӧ�Ծ���H, p2 = H*p1
	Mat3X t(3, 1); t << cam2.t_[0], cam2.t_[1], cam2.t_[2];
	MatXX nt(1, 3); nt << 0, 0, 1. / z;
	H = cam2.K_ * (cam2.R_ + t * nt) * cam1.K_.inverse();
}
