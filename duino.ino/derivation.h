// period is in milliseconds
float derivation(unsigned long period, long err_actual, long err_prev){
  int err = err_actual - err_prev;
  return static_cast<float>(err)/period;
}