#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>


using namespace std;

struct Vec3 {
    double x, y, z;

    Vec3 operator+(const Vec3& b) const {
        return {x + b.x, y + b.y, z + b.z};
    }

    Vec3 operator-(const Vec3& b) const {
        return {x - b.x, y - b.y, z - b.z};
    }

    Vec3 operator*(double s) const {
        return {x * s, y * s, z * s};
    }

    Vec3& operator+=(const Vec3& b) {
        x += b.x; y += b.y; z += b.z;
        return *this;
    }
    
};

Vec3 randomVec3(double min, double max) {
    static std::random_device rd;              // seed source
    static std::mt19937 gen(rd());             // Mersenne Twister engine
    std::uniform_real_distribution<double> dist(min, max);

    return { dist(gen), dist(gen), dist(gen) };
    }

double norm(const Vec3& v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

// Compute accelerations (Newtonian gravity example)
void computeAcceleration(
    const vector<Vec3>& positions,
    const vector<double>& masses,
    vector<Vec3>& accelerations,
    double G)
{
    int N = positions.size();

    for(int i = 0; i < N; i++)
        accelerations[i] = {0.0, 0.0, 0.0}; //initializing Positions in x=0,y=0 , z=0

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(i == j) continue;

            Vec3 r = positions[j] - positions[i];
            double rnorm = norm(r) + 1e-10;  // softening to avoid singularity

            accelerations[i] += r * (G * masses[j] / pow(rnorm, 3));
        }
    }
}

int main() {

    const int N = 10;          // number of particles
    const double dt = 0.0001;  // time step
    const double T = 1.0;    // total time

    const double G = 4 * M_PI * M_PI;
    //cout << G << endl;

    int steps = T / dt;

    vector<Vec3> r(N), v(N), a(N);
    vector<double> m(N);

    // Initial conditions (example)

    for (int i = 0; i<N; i++){
        r[i] = randomVec3(0.0, 4.0);
        v[i] = {0.0, 0.0, 0.0};//randomVec3(0.0, 0.01);//{0.0, 0.0, 0.0};
        m[i] =  randomVec3(0.001, 0.5).x;
    }
    //r[2] = {0.0, 0.0, 1.0};
    //Vec3 pp=randomVec3(-1.0, 1.0);
    //cout << pp.x << endl;
    //r[0] = {0.0, 0.0, 0.0};
    //r[1] = {1.0, 0.0, 0.0};

    //v[0] = {0.0, 0.0, 0.0};
    //v[1] = {0.0, 6.0, 0.0};
    //v[2] = {0.0, 0.0, 0.5};

    //m[0] = 1.0;
    //m[1] = 3.00274e-6;
   //m[2] = 1.0;

    ofstream file("trajectory.csv");
    //file << "t,x1,y1,z1,x2,y2,z2\n";
    file << "t,";

    // x components
    for(int i = 0; i < N; i++) {
        file << "x" << i+1 << ",";
    }

    // y components
    for(int i = 0; i < N; i++) {
        file << "y" << i+1 << ",";
    }

    // z components
    for(int i = 0; i < N; i++) {
        file << "z" << i+1;
        if(i != N-1) file << ",";
    }

    file << "\n";

    for(int step = 0; step < steps; step++) {

        double t = step * dt;

        computeAcceleration(r, m, a, G);

        // Euler update
        // for(int i = 0; i < N; i++) {
        //     v[i] += a[i] * dt;
        //     r[i] += v[i] * dt;
        // }
        // Velocity Verlet update 
        for(int i=0; i <N; i++){
            v[i] += 0.5 * a[i] * dt;
            r[i] += v[i] * dt;
        }

        file << t <<",";

        // x components
        for(int i = 0; i < N; i++) {
            file << r[i].x << ",";
        }

        // y components
        for(int i = 0; i < N; i++) {
            file << r[i].y << ",";
        }

        // z components
        for(int i = 0; i < N; i++) {
            file << r[i].z;
            if(i != N-1) file << ",";
        }

        file << "\n";
        }

    file.close();

    cout << "Simulation complete. Data saved to trajectory.csv\n";
    return 0;
}