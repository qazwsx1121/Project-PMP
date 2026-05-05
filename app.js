function loadPlaces(lat, lng) {
  console.log("🔥 app.js 실행됨");
  console.log("사용 위치:", lat, lng);

  // ⭐ 지도 업데이트
  const frame = document.getElementById("mapFrame");
  if (frame) {
    frame.src = `/dongjak_animal_map.html?lat=${lat}&lng=${lng}`;
  }

  // ⭐ 리스트 업데이트
  fetch(`/api/nearby?lat=${lat}&lng=${lng}`)
    .then(res => {
      if (!res.ok) throw new Error("API 오류");
      return res.json();
    })
    .then(places => {

      const placeList = document.getElementById("placeList");
      if (!placeList) return;

      placeList.innerHTML = "";

      places.forEach((place, index) => {

        const li = document.createElement("li");
        li.className = "place-item";

        const distance = Number(place.distance);

        li.innerHTML = `
          <div class="place-name">
            ${index + 1}. ${place.name}
          </div>
          <div class="place-info">
            거리: ${isNaN(distance) ? "-" : distance.toFixed(2) + " km"} |
            추천 점수: ${place.score}
          </div>
        `;

        li.onclick = () => {
          showDetail(place);

          // ⭐ 클릭하면 지도 이동
          if (frame) {
            frame.src = `/dongjak_animal_map.html?lat=${place.lat}&lng=${place.lng}`;
          }
        };

        placeList.appendChild(li);
      });

    })
    .catch(err => {
      console.error(err);
      alert("데이터 불러오기 실패");
    });
}

window.onload = function () {

  if (!navigator.geolocation) {
    alert("위치 기능 지원 안됨");

    // 👉 fallback 바로 실행
    loadPlaces(37.4963538, 126.9572222);
    return;
  }

  navigator.geolocation.getCurrentPosition(

    (position) => {
      const lat = position.coords.latitude;
      const lng = position.coords.longitude;

      loadPlaces(lat, lng);
    },

    (error) => {
      console.error("위치 오류:", error);
      alert("위치 허용 안됨 → 기본 위치 사용");

      // ⭐ fallback에서도 리스트 + 지도 둘 다 실행
      loadPlaces(37.4963538, 126.9572222);
    }

  );
};