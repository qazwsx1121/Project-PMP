#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// 시설 정보를 담을 구조체
struct Facility {
    string name;
    double lat;
    double lon;
    string info;
};

int main() {
    // 서울 동작구의 동물 관련 시설  (미리 찍어놓은 시설)
    vector<Facility> facilities = {
        {"보라매 동물병원", 37.4952, 126.9178, "24시간 응급 진료 가능"},
        {"노들 펫샵", 37.5125, 126.9535, "프리미엄 수제 간식 및 용품 판매"},
        {"보라매공원 반려견 놀이터", 37.4988, 126.9155, "무료 개방, 대/소형견 분리 (월요일 휴무)"},
        {"상도동 고양이 카페", 37.5020, 126.9450, "입장료 1만원, 음료 포함"}
    };

    // CSV 파일로 데이터 내보내기
    ofstream outFile("facilities.csv");
    if (!outFile.is_open()) {
        cout << "파일을 열 수 없습니다!" << endl;
        return 1;
    }

    // CSV 헤더 작성
    outFile << "Name,Latitude,Longitude,Info\n";

    // 데이터 쓰기
    for (const auto& f : facilities) {
        // 실제 환경에서는 검색, 필터링 로직이 이쯤에 
        outFile << f.name << "," 
                << f.lat << "," 
                << f.lon << "," 
                << f.info << "\n";
    }

    outFile.close();
    cout << "시설 데이터 facilities.csv 저장." << endl; //이 부분은 굳이 안써도 됨. 저장 되었는지 가시적으로 확인하기 위함임. 나중에 지울거

    return 0;
}