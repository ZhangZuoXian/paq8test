#ifndef PAQ8PX_MODELS_HPP
#define PAQ8PX_MODELS_HPP

#include "MatchModel.hpp"

#include "NormalModel.hpp"


/**
 * This is a factory class for lazy object creation for models.
 * Objects created within this class are instantiated on first use and guaranteed to be destroyed.
 */
class Models {
private:
  Shared * const shared;
public:
  explicit Models(Shared* const sh);
  auto normalModel() -> NormalModel &;

  auto matchModel() -> MatchModel &;
 
};

#endif //PAQ8PX_MODELS_HPP
