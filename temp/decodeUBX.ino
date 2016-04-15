void decodeUBX(void)
{
  if (UBX_class == 0x01)
  {
    switch (UBX_id)
    {
      case 0x02:  //NAV-POSLLH
        {
          GPSdata[0] = message[4];    //longitude-0
          GPSdata[1] = message[5];    //longitude-1
          GPSdata[2] = message[6];    //longitude-2
          GPSdata[3] = message[7];    //longitude-3

          GPSdata[4] = message[8];    //latitude-0
          GPSdata[5] = message[9];    //latitude-1
          GPSdata[6] = message[10];   //latitude-2
          GPSdata[7] = message[11];   //latitude-3

          GPSdata[8] = message[16];   //height-0
          GPSdata[9] = message[17];   //height-1
          GPSdata[10] = message[18];  //height-2
          GPSdata[11] = message[19];  //height-3

          GPSdata[12] = message[20];  //horAcc-0
          GPSdata[13] = message[21];  //horAcc-1
          GPSdata[14] = message[22];  //horAcc-2
          GPSdata[15] = message[23];  //horAcc-3

          GPSdata[16] = message[24];  //vertAcc-0
          GPSdata[17] = message[25];  //vertAcc-1
          GPSdata[18] = message[26];  //vertAcc-2
          GPSdata[19] = message[27];  //vertAcc-3

          POSLLH = true;
          break;
        }
      case 0x12:  //NAV-VELNED
        {
          GPSdata[20] = message[16];  //speed-0
          GPSdata[21] = message[17];  //speed-1
          GPSdata[22] = message[18];  //speed-2
          GPSdata[23] = message[19];  //speed-3

          GPSdata[24] = message[20];  //gSpeed-0
          GPSdata[25] = message[21];  //gSpeed-1
          GPSdata[26] = message[22];  //gSpeed-2
          GPSdata[27] = message[23];  //gSpeed-3

          GPSdata[28] = message[24];  //heading-0
          GPSdata[29] = message[25];  //heading-1
          GPSdata[30] = message[26];  //heading-2
          GPSdata[31] = message[27];  //heading-3

          GPSdata[32] = message[28];  //speedAcc-0
          GPSdata[33] = message[29];  //speedAcc-1
          GPSdata[34] = message[30];  //speedAcc-2
          GPSdata[35] = message[31];  //speedAcc-3

          GPSdata[36] = message[32];  //headingAcc-0
          GPSdata[37] = message[33];  //headingAcc-1
          GPSdata[38] = message[34];  //headingAcc-2
          GPSdata[39] = message[35];  //headingAcc-3

          VELNED = true;
          break;
        }
    }
  }
  if (POSLLH == 1 || VELNED == 1)
  {
    mySerial.print("Ypa!");
  }
}
