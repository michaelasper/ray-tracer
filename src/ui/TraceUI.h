//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __TraceUI_h__
#define __TraceUI_h__

#include <memory>
#include <string>
#define MAX_THREADS 32

using std::string;

class RayTracer;
class CubeMap;

class TraceUI {
   public:
    TraceUI();
    virtual ~TraceUI();

    virtual int run() = 0;

    // Send an alert to the user in some manner
    virtual void alert(const string& msg) = 0;

    // setters
    virtual void setRayTracer(RayTracer* r) { raytracer = r; }
    void useCubeMap(bool b) { m_usingCubeMap = b; }

    // accessors:
    int getSize() const { return m_nSize; }
    int getDepth() const { return m_nDepth; }
    int getDivs() const { return m_dDiv; }
    int getBlockSize() const { return m_nBlockSize; }
    bool isThreshold() const { return m_nThreshold > 0; }
    double getThreshold() const { return (double)m_nThreshold * 0.001; }
    double getAaThreshold() const { return (double)m_nAaThreshold * 0.001; }
    double getFocalD() const { return m_focalDistance; }
    double getApSize() const { return m_ApSize; }
    double getMC() const { return m_monte_carlo; }
    double getPathSamples() const { return m_pathSamples; }
    double getLightRadius() const { return m_light_rad; }
    double getLightSamples() const { return m_light_samples; }
    int getSuperSamples() const { return m_nSuperSamples; }
    int getMaxDepth() const { return m_nTreeDepth; }
    int getLeafSize() const { return m_nLeafSize; }
    int getFilterWidth() const { return m_nFilterWidth; }
    int getThreads() const { return m_threads; }
    int get3dMode() const { return m_3d_mode; }
    bool aaSwitch() const { return m_antiAlias; }
    bool ssSwitch() const { return m_stochaticAlias; }
    bool toonSwitch() const { return m_toon; }
    bool dSwitch() const { return m_depthOfField; }
    bool dddSwitch() const { return m_render3d; }
    bool kdSwitch() const { return m_kdTree; }
    bool shadowSw() const { return m_shadows; }
    bool smShadSw() const { return m_smoothshade; }
    bool pathTrace() const { return m_path; }
    bool bkFaceSw() const { return m_backface; }
    bool cubeMap() const { return m_usingCubeMap && cubemap; }
    CubeMap* getCubeMap() const { return cubemap.get(); }
    void setCubeMap(CubeMap* cm);
    void subtractDepth() { m_monte_carlo--; }
    bool internalReflection() const { return m_internalReflection; }
    bool backfaceSpecular() const { return m_backfaceSpecular; }

    // ray counter
    static void addRays(int number, int ctr) {
        if (ctr >= 0) rayCount[ctr] += number;
    }
    static void addRay(int ctr) {
        if (ctr >= 0) rayCount[ctr]++;
    }
    static int getCount(int ctr) { return ctr < 0 ? -1 : rayCount[ctr]; }
    static int getCount() {
        int total = 0;
        for (int i = 0; i < m_threads; i++) total += rayCount[i];
        return total;
    }
    static int resetCount(int ctr) {
        if (ctr < 0) return -1;
        int temp = rayCount[ctr];
        rayCount[ctr] = 0;
        return temp;
    }
    static int resetCount() {
        int total = 0;
        for (int i = 0; i < m_threads; i++) {
            total += rayCount[i];
            rayCount[i] = 0;
        }
        return total;
    }

    static int m_threads;  // number of threads to run
    static bool m_debug;

    static bool matchCubemapFiles(const string& one_cubemap_file,
                                  string matched_fn[6], string& pdir);

   protected:
    RayTracer* raytracer = nullptr;

    int m_nSize = 512;        // Size of the traced image
    int m_nDepth = 0;         // Max depth of recursion
    int m_nThreshold = 0;     // Threshold for interpolation within block
    int m_nBlockSize = 4;     // Blocksize (square, even, power of 2 preferred)
    int m_nSuperSamples = 3;  // Supersampling rate (1-d) for antialiasing
    int m_nAaThreshold =
        100;                 // Pixel neighborhood difference for supersampling
    int m_nTreeDepth = 15;   // maximum kdTree depth
    int m_nLeafSize = 10;    // target number of objects per leaf
    int m_nFilterWidth = 1;  // width of cubemap filter
    int m_3d_mode = 0;

    double m_focalDistance = 1;
    double m_ApSize = 0.3;
    int m_dDiv = 50;

    static int rayCount[MAX_THREADS];  // Ray counter

    // Determines whether or not to show debugging information
    // for individual rays.  Disabled by default for efficiency
    // reasons.
    bool m_displayDebuggingInfo = false;
    bool m_antiAlias = false;  // Is antialiasing on?
    bool m_stochaticAlias = false;
    bool m_kdTree = true;         // use kd-tree?
    bool m_shadows = true;        // compute shadows?
    bool m_smoothshade = true;    // turn on/off smoothshading?
    bool m_backface = true;       // cull backfaces?
    bool m_usingCubeMap = false;  // render with cubemap
    bool m_internalReflection =
        false;  // Enable reflection inside a translucent object.
    bool m_backfaceSpecular =
        false;  // Enable specular component even seeing through the back of a
                // translucent object.
    bool m_render3d = false;
    bool m_depthOfField = false;
    bool m_toon = false;

    // PATH TRACING

    bool m_path = false;
    double m_pathSamples = 20;
    double m_monte_carlo = 2;
    double m_light_rad = 5;
    double m_light_samples = 100;

    std::unique_ptr<CubeMap> cubemap;

    void loadFromJson(const char* file);
    void smartLoadCubemap(const string& file);
};

#endif
