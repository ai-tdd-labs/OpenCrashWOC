/*

if ((WumpaRayCast(src,&end,ratio) != 0) && (temp_ratio < ratio)) {
    temp_ratio = (temp_ratio - (0.1f / length));
    if (0.0f <= temp_ratio) {
      plr_target_found = 1;
      ratio = temp_ratio;
    }
  }
