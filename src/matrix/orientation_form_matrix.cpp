#include "orientation_form_matrix.h"

#include "matrix_utils.h"
using std::vector;

bool sv3d::OrientationFormEssential(const Mat3X& p1, const Mat3X& p2,
											const RMat3& k1_mat, const RMat3& k2_mat,
											const Mat3& E, Mat3& R, Vec3& t,
                                            const double uniqueness_ratio)
{
	assert(p1.rows() == p2.rows());
	assert(p1.cols() == p2.cols());

	// �ֽ�E����õ�R,t��ѡֵ
	std::vector<Mat3> R_vec;
	std::vector<Vec3> t_vec;
	SolveRtFromEssential(E,R_vec,t_vec);
	if(R_vec.size()!=2u || t_vec.size()!=2) {
		return false;
	}
	
	// R,t������ֵ����4�����
	// �������������ǰ���Ĺ۲�����������������Ϊ���Ž�
	struct Pose {
		Mat3* R;
		Vec3* t;
		Pose(Mat3& R_,Vec3& t_):R(&R_),t(&t_) {}
	};
	vector<Pose> pose_vec;
	pose_vec.emplace_back(R_vec[0], t_vec[0]); pose_vec.emplace_back(R_vec[0], t_vec[1]);
	pose_vec.emplace_back(R_vec[1], t_vec[0]); pose_vec.emplace_back(R_vec[1], t_vec[1]);

	
	Mat3 R0; R0.setIdentity();
	Vec3 t0; t0.setZero();

	vector<unsigned> num_front_points(4);		// ������˫���ǰ���ĵ���

	// ѡ��˫���ǰ����������Pose��Ϊ���ŵ�Pose
	unsigned max_front_points = 0;
	int max_idx = -1;
	for (int k = 0; k < 4; k++) {
		auto& pose = pose_vec[k];
		const auto num_pts = p1.cols();
		for(unsigned n = 0u; n < num_pts;n++) {
			const Vec3& x1 = p1.col(n);
			const Vec3& x2 = p2.col(n);
			Vec3 X;
			// ���ǻ�����ռ�����꣬���жϿռ�������Ƿ����������ǰ��
			if(Triangulate2View(x1, k1_mat, R0, t0, x2, k2_mat, *pose.R, *pose.t, X)) {
				++num_front_points[k];
			}
		}
		if(max_front_points < num_front_points[k]) {
			max_front_points = num_front_points[k];
			max_idx = k;
		}
	}

	if (max_idx == -1)
		return false;

	// ������ŵ�R,t
	R = *pose_vec[max_idx].R;
	t = *pose_vec[max_idx].t;

	// ����Ψһ��
	std::sort(num_front_points.begin(), num_front_points.end());

	const double ratio = num_front_points.rbegin()[1] / static_cast<double>(num_front_points.rbegin()[0]);
	return ratio < uniqueness_ratio;
}
