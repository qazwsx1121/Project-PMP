#include <crow/crow.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#define M_PI 3.14159265358979323846


double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

double getDistance(double lat1, double lng1, double lat2, double lng2) {
    double dLat = deg2rad(lat2 - lat1);
    double dLng = deg2rad(lng2 - lng1);

    lat1 = deg2rad(lat1);
    lat2 = deg2rad(lat2);

    double a = sin(dLat / 2) * sin(dLat / 2) +
        sin(dLng / 2) * sin(dLng / 2) * cos(lat1) * cos(lat2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double R = 6371; // km

    return R * c;
}

std::string readFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return readFile("static/index.html");
        });

    CROW_ROUTE(app, "/app.js")([]() {
        return readFile("static/app.js");
        });

    CROW_ROUTE(app, "/dongjak_animal_map.html")([]() {
        return readFile("static/dongjak_animal_map.html");
        });

    CROW_ROUTE(app, "/api/places")([]() {

        std::ifstream file("data/places.csv");
        std::string line;

        crow::json::wvalue result;
        int index = 0;

        getline(file, line); 

        while (getline(file, line))
        {
            std::stringstream ss(line);
            std::string name, lat, lng, score, light, crowd, reason;

            getline(ss, name, ',');
            getline(ss, lat, ',');
            getline(ss, lng, ',');
            getline(ss, score, ',');
            getline(ss, light, ',');
            getline(ss, crowd, ',');
            getline(ss, reason, ',');

            result[index]["name"] = name;
            result[index]["lat"] = std::stod(lat);
            result[index]["lng"] = std::stod(lng);
            result[index]["score"] = std::stod(score);
            result[index]["light"] = light;
            result[index]["crowd"] = crowd;
            result[index]["reason"] = reason;

            index++;
        }

        return result;
        });


    CROW_ROUTE(app, "/api/nearby")
        ([](const crow::request& req) {

        auto lat = req.url_params.get("lat");
        auto lng = req.url_params.get("lng");

        if (!lat || !lng) {
            crow::json::wvalue err;
            err["error"] = "missing parameters";
            return err;
        }

        double userLat = lat ? std::stod(lat) : 0;
        double userLng = lng ? std::stod(lng) : 0;

        std::cout << "UserLat: " << userLat << ", UserLng: " << userLng << std::endl;

        std::ifstream file("data/places.csv");
        std::string line;

        getline(file, line); // header skip

        struct Place {
            std::string name;
            double lat, lng;
            double score;
            std::string light, crowd, reason;
            double distance;
        };

        std::vector<Place> places;

        while (getline(file, line))
        {
            std::stringstream ss(line);

            std::string name, latStr, lngStr, scoreStr, light, crowd, reason;

            getline(ss, name, ',');
            getline(ss, latStr, ',');
            getline(ss, lngStr, ',');
            getline(ss, scoreStr, ',');
            getline(ss, light, ',');
            getline(ss, crowd, ',');
            getline(ss, reason, ',');

            Place p;
            p.name = name;
            p.lat = std::stod(latStr);
            p.lng = std::stod(lngStr);
            p.score = std::stod(scoreStr);
            p.light = light;
            p.crowd = crowd;
            p.reason = reason;

            // 🔥 거리 계산
            p.distance = getDistance(userLat, userLng, p.lat, p.lng);

            places.push_back(p);
        }

        // 🔥 거리 기준 정렬
        std::sort(places.begin(), places.end(), [](const Place& a, const Place& b) {
            return a.distance < b.distance;
            });

        crow::json::wvalue result;

        int limit = std::min(5, (int)places.size());

        for (int i = 0; i < limit; i++) {
            result[i]["name"] = places[i].name;
            result[i]["lat"] = places[i].lat;
            result[i]["lng"] = places[i].lng;
            result[i]["distance"] = places[i].distance;
            result[i]["score"] = places[i].score;
            result[i]["light"] = places[i].light;
            result[i]["crowd"] = places[i].crowd;
            result[i]["reason"] = places[i].reason;
        }

        return result;
            });
  
    app.bindaddr("0.0.0.0").port(18080).run(); 
}