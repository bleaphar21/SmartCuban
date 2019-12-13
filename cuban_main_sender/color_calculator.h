/*
 * Credit to Eran Kampf for the logic to convert Celcius to an RGB color value.
 * Repo can be found here: https://gist.github.com/ekampf/7721226
 */

class ColorCalculator {
  public:  

    ColorCalculator(){}
    
    void populateWeatherColors(float temp, int& red, int& green, int& blue) {
      float wav = wavelength_for_point(temp, minTemp, maxTemp);
      wavelength_to_rgb(wav, red, green, blue);
    }

  private:
    const float minTemp = -20.0;
    const float maxTemp = 50.0;
    const float Gamma = 0.80;
    const int   IntensityMax = 255;
    
    float wavelength_for_point(float v, float minValue, float maxValue) {
      const float minVisibleWaveLength = 350.0;
      const float maxVisibleWaveLength = 650.0;
  
      //Convert data value in the range of MinValues..MaxValues to the 
      //range 350..650
      float range = maxValue - minValue;
   
      float result = (v - minValue) / range;
      result = result * (maxVisibleWaveLength - minVisibleWaveLength) + minVisibleWaveLength;
      return result;
    }

    void wavelength_to_rgb(float wavelength, int& rV, int& gV, int& bV) {
      int w = int(wavelength);
      
      float r;
      float g;
      float b;
      float SSS;
      
  
      // colour
      if (w >= 380 && w < 440) {
          r = -(w - 440.0) / (440.0 - 350.0);
          g = 0.0;
          b = 1.0;
      } 
      else if (w >= 440 && w < 490) {
          r = 0.0;
          g = (w - 440.0) / (490.0 - 440.0);
          b = 1.0;
      }
      else if (w >= 490 && w < 510) {
          r = 0.0;
          g = 1.0;
          b = -(w - 510.) / (510. - 490.);
      }
      else if (w >= 510 && w < 580) {
          r = (w - 510.) / (580. - 510.);
          g = 1.0;
          b = 0.0;
      }
      else if (w >= 580 && w < 645) {
          r = 1.0;
          g = -(w - 645.) / (645. - 580.);
          b = 0.0;
      }
      else if (w >= 645 && w <= 780) {
          r = 1.0;
          g = 0.0;
          b = 0.0;
      }
      else {
          r = 0.0;
          g = 0.0;
          b = 0.0;
      }
  
      // intensity correction
      if (w >= 380 && w < 420) {
          SSS = 0.3 + 0.7*(w - 350) / (420 - 350);
      }
      else if (w >= 420 && w <= 700) {
          SSS = 1.0;
      }
      else if (w > 700 && w <= 780) {
          SSS = 0.3 + 0.7*(780 - w) / (780 - 700);
      }
      else {
        SSS = 0.0;
      }
      
      SSS *= 255;
  
      rV = SSS*r;
      gV = SSS*g;
      bV = SSS*b;
  }
    
};
