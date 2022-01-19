#ifndef PAQ8PX_MODELS_HPP
#define PAQ8PX_MODELS_HPP

#include "./model/MatchModel.hpp"
#include "./model/NormalModel.hpp"
#include "./model/SparseModel.hpp"

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

  auto sparseModel() -> SparseModel &;
};

#endif //PAQ8PX_MODELS_HPP
