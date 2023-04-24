#include "ContextModel.hpp"

ContextModel::ContextModel(Shared* const sh,Models& models):shared(sh),models(models){
    m = new SIMDMixer<SIMD_NONE>(sh, 
    1+
    MatchModel::MIXERINPUTS + NormalModel::MIXERINPUTS+SparseModel::MIXERINPUTS,
    MatchModel::MIXERCONTEXTS + NormalModel::MIXERCONTEXTS+SparseModel::MIXERCONTEXTS,
    MatchModel::MIXERCONTEXTSETS + NormalModel::MIXERCONTEXTSETS+SparseModel::MIXERCONTEXTSETS
    );
}


auto ContextModel::p() -> int {
  
  // Parse block type and block size
  
  
  m->add(256); //network bias

  // clock_t t = clock();

  MatchModel &matchModel = models.matchModel();
  matchModel.mix(*m);
  // shared->MatchModelCostTime +=  clock()-t;

  // t = clock();
  NormalModel &normalModel = models.normalModel();
  normalModel.mix(*m);
  normalModel.mixPost(*m);
  // shared->NormalModelCostTime += clock()-t;

  SparseModel &sparseModel = models.sparseModel();
  sparseModel.mix(*m);

  m->setScaleFactor(1024, 128);
  // return 2048;
  return m->p();

}

auto ContextModel::pWPar(uint8_t c) -> int*{
  int pr[8]={2048};
  //model的set、update、以及mix处理
  
  //map的mix、update以及update处理
  
    

  // return pr;
  return nullptr;
}

ContextModel::~ContextModel() {
  delete m;
}

