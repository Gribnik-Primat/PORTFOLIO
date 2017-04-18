  /* Опрос устройств ввода */
  if ((i = joyGetNumDevs()) > 1)
  {
    JOYCAPS jc;

    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(ji);
      ji.dwFlags = JOY_RETURNALL;

      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* кнопки */
        memcpy(VG4_Anim.JButOld, VG4_Anim.JBut, 32);
        for (i = 0; i < 32; i++)
          VG4_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        /* оси переводим в диапазон -1..1 */
        VG4_Anim.Jx = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin) - 1;
        VG4_Anim.Jy = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin) - 1;
        VG4_Anim.Jz = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin) - 1;
        VG4_Anim.Jr = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin) - 1;

        if (ji.dwPOV == 0xFFFF)
          VG4_Anim.JPov = 0;
        else
          VG4_Anim.JPov = ji.dwPOV / 4500 + 1;
      }
    }
  }
