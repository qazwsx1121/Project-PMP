import folium
import csv

def create_map():
    # 학교 기준
    soongsil_univ = [37.4963538, 126.9572222]
    
    # Folium 지도 객체 생성 (숭실대 기준)
    m = folium.Map(location=soongsil_univ, zoom_start=14)

    # 2. 숭실대학교 핑
    # popup=folium.Popup(..., show=True)를 쓰면 지도를 열자마자 말풍선이 떠있습니다.
    folium.Marker(
        location=soongsil_univ,
        popup=folium.Popup("<b>📍 현재 위치: 숭실대학교</b>", show=True, max_width=200),
        tooltip="숭실대학교 (중심지)",
        icon=folium.Icon(
            color='darkred', icon='university',
            prefix='fa'           # FontAwesome 아이콘 사용
        )
    ).add_to(m)
    
    # C++에서 생성한 CSV 파일 읽기
    try:
        with open('facilities.csv', mode='r', encoding='EUC-KR') as file:
            reader = csv.DictReader(file)
            
            for row in reader:
                name = row['Name']
                lat = float(row['Latitude'])
                lon = float(row['Longitude'])
                info = row['Info']
                
                # 팝업에 들어갈 HTML 텍스트 구성
                popup_html = f"<b>{name}</b><br>{info}"
                
                # 핑 추가
                folium.Marker(
                    location=[lat, lon],
                    popup=folium.Popup(popup_html, max_width=300),
                    tooltip=name,
                    icon=folium.Icon(color='blue', icon='info-sign')
                ).add_to(m)
                
    except FileNotFoundError:
        print("facilities.csv 파일을 찾을 수 없습니다. C++ 프로그램을 먼저 실행해주세요.")
        return

    # 지도를 HTML 파일로 저장
    m.save('dongjak_animal_map.html')
    print("지도: dongjak_animal_map.html 생성 -완-")

if __name__ == "__main__":
    create_map()