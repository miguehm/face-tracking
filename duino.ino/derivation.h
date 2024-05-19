// period is in milliseconds
float derivation(int period, int err_actual, int err_prev){
  int err = err_actual - err_prev;
  return static_cast<float>(err)/period;
}