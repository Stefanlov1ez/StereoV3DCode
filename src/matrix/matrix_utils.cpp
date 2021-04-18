#include "matrix_utils.h"

void sv3d::SolveRtFromEssential(const Mat3& E, std::vector<Mat3>& R_vec, std::vector<Vec3>& t_vec)
{
	// Multiple View Geometry in Computer Vision, chapter 9.7 page 259
	// �Ա��ʾ������SVD�ֽ�
	Eigen::JacobiSVD<Mat3> USV(E, Eigen::ComputeFullU | Eigen::ComputeFullV);
	Mat3 U = USV.matrixU();
	Mat3 Vt = USV.matrixV().transpose();

	// ����E����������ֵ��ȣ�����U��Vt��Ψһ������ΪU�ĵ�ǰ���п��Ի�����Vt��ǰ���п��Ի���
	// �л������л��������Ľ��������ʽ��Ϊ�෴��
	// ����תR���������ʽ��+1,W������ʽΪ+1,��R = UWVt or UWtVt, ���UV������ʽ���Ų���ͬ�����R����ʽ��-1���Ǵ����
	// ����ǿ����U��Vt������ʽ��Ϊ��ֵ�����ж�U��Vt����ʽΪ��ʱ����U�����һ�к�Vt�����һ�з�����һ���෴��������ʽ��Ϊ��ֵ
	if (U.determinant() < 0) {
		U.col(2) *= -1;
	}
	// Last row of Vt is undetermined since d = (a a 0).
	if (Vt.determinant() < 0) {
		Vt.row(2) *= -1;
	}
	
	Mat3 W;
	W << 0, -1,  0,
		 1,  0,  0,
		 0,  0,  1;

	auto d1 = E.determinant();
	auto d2 = U.determinant();
	auto d3 = Vt.determinant();
	auto d4 = W.determinant();
	
	R_vec.clear();
	t_vec.clear();

	// R1 = U*Wt*Vt, R2 = U*W*Vt
	R_vec.emplace_back(U * W * Vt);
	R_vec.emplace_back(U* W.transpose() *Vt);

	auto d5 = R_vec[0].determinant();
	auto d6 = R_vec[1].determinant();
	
	// t1^ = U*Z*Ut = S1, t2^ = U*Zt*Ut = -t1^
	// S1*t1 = 0 -> U*Z*Ut*t1=0 -> Z*(Ut*t1)=0
	// -> Ut*t1 = (0,0,1)T -> t1 = U*(0,0,1)T = U.col(2)
	// t2 = -t1
	t_vec.emplace_back(U.col(2));
	t_vec.emplace_back(-U.col(2));
}

bool sv3d::Triangulate2View(const Vec3& p1, const RMat3& K1, const Mat3& R1, const Vec3& t1,
							const Vec3& p2, const RMat3& K2, const Mat3& R2, const Vec3& t2,
							Vec3& X)
{
	// ˫Ŀǰ�����������������ϵ�µĿռ������

	// ����ͶӰ����
	Mat34 P1, P2;
	P1.block<3, 3>(0, 0) = R1;
	P2.block<3, 3>(0, 0) = R2;
	P1.block<3, 1>(0, 3) = t1;
	P2.block<3, 1>(0, 3) = t2;
	P1 = K1 * P1;
	P2 = K2 * P2;
	
	// DLTֱ�����Ա任�����
	// dp = PX -> 0 = cross(p,dp) = cross(p,PX) -> cross(p,P)X = 0 -> AX = 0, A = cross(p,P)
	// cross(p1,P1)X = 0
	// cross(p2,P2)X = 0
	Mat4 A;
	A.row(0) = p1[1] * P1.row(2) - p1[2] * P1.row(1);
	A.row(1) = p1[2] * P1.row(0) - p1[0] * P1.row(2);
	A.row(2) = p2[1] * P2.row(2) - p2[2] * P2.row(1);
	A.row(3) = p2[2] * P2.row(0) - p2[0] * P2.row(2);

	// SVD�ֽ����V�����һ�У���ATA����С����ֵ��Ӧ����������
	JacobiSVD<Mat4> svd(A, ComputeFullV);
	X = (svd.matrixV().col(3)).hnormalized();
	
	return (R1 * X + t1)[2] > 0.0 && (R2 * X + t2)[2] > 0.0;
}
