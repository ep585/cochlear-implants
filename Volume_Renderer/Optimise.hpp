#ifndef Optimise_hpp
#define Optimise_hpp

#include "vec2.hpp"
#include "vec3.hpp"
#include "Data.hpp"

namespace Optimise {

struct OpVals {
	Params params;
	float f;
};

OpVals Scan(float (*const &similarityMetric)(const Array2D<uint16_t> &, const Array2D<uint16_t> &, bool (*const &)(const int &, const int &)), const char *fileName, const bool &checkFeasible);

struct PSParams {
	int N; // number of particles
	float w, phiP, phiG; // inertia weight, cognitive coefficient, social coefficient
	
	// operator overloading for easy printing of parameters
	friend std::ostream &operator<<(std::ostream &stream, const PSParams &params){
		return stream << "{" << params.N << ", " << params.w << ", " << params.phiP << ", " << params.phiG << "}";
	}
};
struct Particle {
	Particle(){}
	Particle(float (*const &similarityMetric)(const Array2D<uint16_t> &, const Array2D<uint16_t> &, bool (*const &)(const int &, const int &)), OpVals &best, const Params &params0, const Params &paramsRange, const bool &checkFeasible);
	
	void Update(float (*const &similarityMetric)(const Array2D<uint16_t> &, const Array2D<uint16_t> &, bool (*const &)(const int &, const int &)), OpVals &best, const PSParams &psParams, const bool &checkFeasible);
	
	void ReCalc(float (*const &similarityMetric)(const Array2D<uint16_t> &, const Array2D<uint16_t> &, bool (*const &)(const int &, const int &)), OpVals &best);
	
	Params v;
	OpVals current;
	OpVals myBest;
};
OpVals ParticleSwarm(float (*const &similarityMetric)(const Array2D<uint16_t> &, const Array2D<uint16_t> &, bool (*const &)(const int &, const int &)), const int &iterations, const PSParams &psParams, const Params &params0, const Params &paramsRange, const bool &checkFeasible=true, Particle *particles=nullptr);

struct PlotRanges {
	float tr1, tr2, rot;
	friend PlotRanges operator*(const float &f, const PlotRanges &pr){ return {f*pr.tr1, f*pr.tr2, f*pr.rot}; }
};
void Lines(float (*const &similarityMetric)(const Array2D<uint16_t> &, const Array2D<uint16_t> &, bool (*const &)(const int &, const int &)), const unsigned short &halfN, const Params &params0, const PlotRanges &ranges/*, const bool &checkFeasible*/, const char *const &fileName);

OpVals LocalSearch(float (*const &similarityMetric)(const Array2D<uint16_t> &, const Array2D<uint16_t> &, bool (*const &)(const int &, const int &)), const Params &params0, const Params &stepSize0, const char *fileName=nullptr);

double TimeDRR();
double TimeSimMetric(float (*const &similarityMetric)(const Array2D<uint16_t> &, const Array2D<uint16_t> &, bool (*const &)(const int &, const int &)));

}

#endif /* Optimise_hpp */
