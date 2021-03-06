#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include <numeric>
#include <vector>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s)
{
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos)
  {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos)
  {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  // Create PID object
  PID pid;

  // Create proportional, derivative and integral variables
  double Kp_twiddle = 0.160013;   // parameters found with twiddle
  double Ki_twiddle = 0.00500106; // parameter found with twiddle
  double Kd_twiddle = 5.00106;    // parameters found with twiddle

  // Create proportional, derivative and integral variables
  double Kp = 0.15;  // parameters found with twiddle
  double Ki = 0.005; // parameter found with twiddle
  double Kd = 5;     // parameters found with twiddle

  // Initialize the pid variable.
  pid.Init(Kp_twiddle, Ki_twiddle, Kd_twiddle);
  // pid.Init(Kp, Ki, Kd);

  // Twiddle parameters
  std::vector<double> p = {0.15, 0.005, 5.0};
  std::vector<double> dp = {0.01, 0.001, 1};
  double best_error = 3.0;
  double tolerance = 0.001;
  int twiddle = 0;

  h.onMessage([&pid, &p, &dp, &best_error, &tolerance, &Kp, &Ki, &Kd, &twiddle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                                                                                uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(string(data).substr(0, length));

      if (s != "")
      {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry")
        {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value;

          if (twiddle == 1)
          {
            while ((dp[0] + dp[1] + dp[2]) > tolerance)
            {
              std::cout << "Twiddle Running\n";
              for (unsigned int i = 0; i < 3; ++i)
              {
                p[i] += dp[i];
                std::cout << "p" << i << " = " << p[i] << "\n";
                pid.UpdateError(cte);
                steer_value = pid.TotalError();
                if (cte < best_error)
                {
                  best_error = cte;
                  dp[i] *= 1.1;
                }
                else
                {
                  p[i] -= 2 * dp[i];
                  // pid.Init(p[0], p[1], p[2]);
                  pid.UpdateError(cte);
                  steer_value = pid.TotalError();
                  if (cte < best_error)
                  {
                    best_error = cte;
                    dp[i] *= 1.1;
                  }
                  else
                  {
                    p[i] += dp[i];
                    dp[i] *= 0.9;
                  }
                }
              }
            }
            std::cout << "\nTwiddle Completed!\n";
            twiddle = 0;
          }

          //Calculate steering value
          pid.UpdateError(cte);
          steer_value = pid.TotalError();

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        } // end "telemetry" if
      }
      else
      {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    } // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }

  h.run();
}