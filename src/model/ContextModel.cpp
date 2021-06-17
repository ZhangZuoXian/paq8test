#include "ContextModel.hpp"

ContextModel::ContextModel(Shared* const sh,Models& models):shared(sh),models(models){
    m = new SIMDMixer<SIMD_NONE>(sh, 
    1+
    MatchModel::MIXERINPUTS + NormalModel::MIXERINPUTS,
    MatchModel::MIXERCONTEXTS + NormalModel::MIXERCONTEXTS,
    MatchModel::MIXERCONTEXTSETS + NormalModel::MIXERCONTEXTSETS
    );
}


auto ContextModel::p() -> int {
  
  // Parse block type and block size
  
  
  m->add(256); //network bias

  MatchModel &matchModel = models.matchModel();
  matchModel.mix(*m);
  NormalModel &normalModel = models.normalModel();
  normalModel.mix(*m);

  normalModel.mixPost(*m);

  m->setScaleFactor(1024, 128);
  // return 2048;
  return m->p();

}

ContextModel::~ContextModel() {
  delete m;
}

