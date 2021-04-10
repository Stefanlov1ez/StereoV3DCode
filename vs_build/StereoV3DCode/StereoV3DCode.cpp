/* -*-c++-*- StereoV3D - Copyright (C) 2021.
* Author	: Ethan Li<ethan.li.whu@gmail.com>
* https://github.com/ethan-li-coding/StereoV3DCode
*/

#include <chrono>
#include <iostream>
#include <string>


#include "../../src/matrix/essential.h"
#include "../../src/matrix/homography.h"
#include "../../src/test/datasets.h"
#include "camera.h"

using namespace std;
using namespace sv3d;
using namespace std::chrono;

void TestEssentialSolving();
void TestHomographySolving();

std::string introduction = "> �װ���ͬѧ�ǣ����ǵ�������3D���磬���ǵ�˫���ܹ��۲���ά��Ϣ���������Ǹ�֪���룬�������ϣ��Ӷ����������֮�䡣���������������ܻ������磬���ǵĿ�ѧ����̽�����ֻ������ܼ������û����ܹ�ӵ���������ά��֪��������ϣ�����ٶȺ;����ϳ�Խ���࣬�����Զ���ʻ�����еĶ�λ���������˻����Զ����ϣ��������е���άɨ��ȣ����Ǹ��ǻ������ܼ�����3D�Ӿ��ϵľ���ʵ�֡�\n\n�����Ӿ�����ά�ؽ��������Ҫ������ģ�����۽ṹ��˫���ģ��˫Ŀ����͸��ͶӰ�����ǲ���Ϊ������ͨ���߼����ӵ�ͬ���������㷨���ָ������е���ά��Ϣ������Ӧ��ʮ��֮�㷺���Զ���ʻ���������ϡ������ؽ�������ʶ������߿Ƽ�Ӧ�ö������ؼ�����Ӱ��\n\n���γ̽��������ǳ������˽������Ӿ���������ʵ��֪ʶ�����ǻ������ϵ��������궨���ӱ���ʽ���彲������ʽ���壬�������ܴ���Ȼָ��������񹹽��봦������Ȥ��ͬѧ�ǣ�������һ��̽�������Ӿ��������ɣ�\n\n���γ��ǵ�����Դ���������Ĳ�������̫����������Լ�����������߼�������ǳ���׶�ΪĿ�꣬ˮƽ���ޣ����в���֮�������벻�ߴͽ̣�\n\n����΢�ţ�EthanYs6�������������������Ⱥ StereoV3D��һ�������ġ�\n\nCSDN������Ethan Li ��ӭ�ɣ��鿴��ҳ��γ̡�\n\n��δ��룬���ϴ���github�ϣ���ַ��https://github.com/ethan-li-coding/StereoV3DCode";
int main()
{
	cout << introduction << endl << endl << endl;

	// ���Ա��ʾ������
	cout << "�����������ԡ���ƪ 2�����ʾ������ ������" << endl << endl;
	TestEssentialSolving();

	// ���Ե�Ӧ�Ծ������
	cout << "�����������ԡ���ƪ 2����Ӧ������� ������" << endl << endl;
	TestHomographySolving();
	
    return 0;
}

void TestEssentialSolving()
{
	// ����ͬ���㼯
	sv3d::SimulativeStereoDataset datasets;
	Mat3X p1, p2;
	const unsigned kPairsCount = 20;
	datasets.GenarateHomonymyPairs(kPairsCount, p1, p2);
	cout << kPairsCount << " pairs of homonymous points have been generated!" << endl << endl;

	// ���㱾�ʾ���
	cout << "Start Solving Essential Matrix......" << endl;
	auto start = steady_clock::now();

	Essential essential;
	essential.Solve(p1, p2, datasets.cam1.K_, datasets.cam2.K_, Essential::EIGHT_POINTS);
	Mat3 Es = essential.Value();

	auto end = steady_clock::now();
	auto tt = duration_cast<microseconds>(end - start);

	double diff = 0.0;
	const double scale = datasets.E.data()[0] / Es.data()[0];
	Es *= scale;
	for (int i = 0; i < 9; i++) {
		diff = abs(Es.data()[i] - datasets.E.data()[i]);
	}
	diff /= 9;
	cout << "Done! Solving Mean Error = " << diff << "   Timing: " << tt.count() / 1000.0 << "ms" << endl << endl;

}

void TestHomographySolving()
{
	// ����ͬһ��ƽ���ڵ�ͬ���㼯
	sv3d::SimulativeStereoDataset datasets;
	Mat3X p1, p2;
	Mat3 H;
	const unsigned kPairsCount = 20;
	datasets.GenarateHomonymyPairsInPlane(kPairsCount, p1, p2, H);
	cout << kPairsCount << " pairs of homonymous points in a plane have been generated!" << endl << endl;

	// ���㵥Ӧ�Ծ���
	cout << "Start Solving Homography Matrix......" << endl;
	auto start = steady_clock::now();

	Homography homography;
	homography.Solve(p1, p2);
	Mat3 Ho = homography.Value();

	auto end = steady_clock::now();
	auto tt = duration_cast<microseconds>(end - start);

	double diff = 0.0;
	const double scale = H.data()[0] / Ho.data()[0];
	Ho *= scale;
	for (int i = 0; i < 9; i++) {
		diff = abs(Ho.data()[i] - H.data()[i]);
	}
	diff /= 9;
	cout << "Done! Solving Mean Error = " << diff << "   Timing: " << tt.count() / 1000.0 << "ms" << endl << endl;

}