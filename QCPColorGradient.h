#ifndef QCPCOLORGRADIENT_H
#define QCPCOLORGRADIENT_H
#include <QMetaType>
#include <QMap>
#include <QColor>
#include <QVector>
#include <QDebug>
#include <QtMath>

class QCPRange
{
public:
  double lower, upper;

  QCPRange()
      :lower(0),
       upper(0)
  {}
  QCPRange(double lower, double upper)
      : lower(lower),
        upper(upper)
  {}

  bool operator==(const QCPRange& other) const { return lower == other.lower && upper == other.upper; }
  bool operator!=(const QCPRange& other) const { return !(*this == other); }

  QCPRange &operator+=(const double& value) { lower+=value; upper+=value; return *this; }
  QCPRange &operator-=(const double& value) { lower-=value; upper-=value; return *this; }
  QCPRange &operator*=(const double& value) { lower*=value; upper*=value; return *this; }
  QCPRange &operator/=(const double& value) { lower/=value; upper/=value; return *this; }
  friend inline const QCPRange operator+(const QCPRange&, double);
  friend inline const QCPRange operator+(double, const QCPRange&);
  friend inline const QCPRange operator-(const QCPRange& range, double value);
  friend inline const QCPRange operator*(const QCPRange& range, double value);
  friend inline const QCPRange operator*(double value, const QCPRange& range);
  friend inline const QCPRange operator/(const QCPRange& range, double value);

  double size() const { return upper-lower; }
  double center() const { return (upper+lower)*0.5; }
  void normalize() { if (lower > upper) qSwap(lower, upper); }
  void expand(const QCPRange &otherRange);
  void expand(double includeCoord);
  QCPRange expanded(const QCPRange &otherRange) const;
  QCPRange expanded(double includeCoord) const;
  QCPRange bounded(double lowerBound, double upperBound) const;
  QCPRange sanitizedForLogScale() const;
  QCPRange sanitizedForLinScale() const;
  bool contains(double value) const { return value >= lower && value <= upper; }

  static bool validRange(double lower, double upper);
  static bool validRange(const QCPRange &range);
  static const double minRange;
  static const double maxRange;

};
Q_DECLARE_TYPEINFO(QCPRange, Q_MOVABLE_TYPE);

/*! \relates QCPRange

  Prints \a range in a human readable format to the qDebug output.
*/
inline QDebug operator<< (QDebug d, const QCPRange &range)
{
    d.nospace() << "QCPRange(" << range.lower << ", " << range.upper << ")";
    return d.space();
}

/*!
  Adds \a value to both boundaries of the range.
*/
inline const QCPRange operator+(const QCPRange& range, double value)
{
  QCPRange result(range);
  result += value;
  return result;
}



/*!
  Adds \a value to both boundaries of the range.
*/
inline const QCPRange operator+(double value, const QCPRange& range)
{
  QCPRange result(range);
  result += value;
  return result;
}

/*!
  Subtracts \a value from both boundaries of the range.
*/
inline const QCPRange operator-(const QCPRange& range, double value)
{
  QCPRange result(range);
  result -= value;
  return result;
}

/*!
  Multiplies both boundaries of the range by \a value.
*/
inline const QCPRange operator*(const QCPRange& range, double value)
{
  QCPRange result(range);
  result *= value;
  return result;
}

/*!
  Multiplies both boundaries of the range by \a value.
*/
inline const QCPRange operator*(double value, const QCPRange& range)
{
  QCPRange result(range);
  result *= value;
  return result;
}

/*!
  Divides both boundaries of the range by \a value.
*/
inline const QCPRange operator/(const QCPRange& range, double value)
{
  QCPRange result(range);
  result /= value;
  return result;
}



class QCPColorGradient
{
  Q_GADGET
public:
  /*!
    Defines the color spaces in which color interpolation between gradient stops can be performed.

    \see setColorInterpolation
  */
  enum ColorInterpolation { ciRGB  ///< Color channels red, green and blue are linearly interpolated
                            ,ciHSV ///< Color channels hue, saturation and value are linearly interpolated (The hue is interpolated over the shortest angle distance)
                          };
  Q_ENUMS(ColorInterpolation)

  /*!
    Defines how NaN data points shall appear in the plot.

    \see setNanHandling, setNanColor
  */
  enum NanHandling { nhNone ///< NaN data points are not explicitly handled and shouldn't occur in the data (this gives slight performance improvement)
                     ,nhLowestColor  ///< NaN data points appear as the lowest color defined in this QCPColorGradient
                     ,nhHighestColor ///< NaN data points appear as the highest color defined in this QCPColorGradient
                     ,nhTransparent ///< NaN data points appear transparent
                     ,nhNanColor ///< NaN data points appear as the color defined with \ref setNanColor
                   };
  Q_ENUMS(NanHandling)

  /*!
    Defines the available presets that can be loaded with \ref loadPreset. See the documentation
    there for an image of the presets.
  */
  enum GradientPreset { gpGrayscale  ///< Continuous lightness from black to white (suited for non-biased data representation)
                        ,gpHot       ///< Continuous lightness from black over firey colors to white (suited for non-biased data representation)
                        ,gpCold      ///< Continuous lightness from black over icey colors to white (suited for non-biased data representation)
                        ,gpNight     ///< Continuous lightness from black over weak blueish colors to white (suited for non-biased data representation)
                        ,gpCandy     ///< Blue over pink to white
                        ,gpGeography ///< Colors suitable to represent different elevations on geographical maps
                        ,gpIon       ///< Half hue spectrum from black over purple to blue and finally green (creates banding illusion but allows more precise magnitude estimates)
                        ,gpThermal   ///< Colors suitable for thermal imaging, ranging from dark blue over purple to orange, yellow and white
                        ,gpPolar     ///< Colors suitable to emphasize polarity around the center, with blue for negative, black in the middle and red for positive values
                        ,gpSpectrum  ///< An approximation of the visible light spectrum (creates banding illusion but allows more precise magnitude estimates)
                        ,gpJet       ///< Hue variation similar to a spectrum, often used in numerical visualization (creates banding illusion but allows more precise magnitude estimates)
                        ,gpHues      ///< Full hue cycle, with highest and lowest color red (suitable for periodic data, such as angles and phases, see \ref setPeriodic)
                      };
  Q_ENUMS(GradientPreset)

  QCPColorGradient()
      : mLevelCount(350),
        mColorInterpolation(ciRGB),
        mNanHandling(nhNone),
        mNanColor(Qt::black),
        mPeriodic(false),
        mColorBufferInvalidated(true)
  {
      mColorBuffer.fill(qRgb(0, 0, 0), mLevelCount);
  }
  QCPColorGradient(GradientPreset preset)
      :mLevelCount(350),
        mColorInterpolation(ciRGB),
        mNanHandling(nhNone),
        mNanColor(Qt::black),
        mPeriodic(false),
        mColorBufferInvalidated(true)
      {
        mColorBuffer.fill(qRgb(0, 0, 0), mLevelCount);
        loadPreset(preset);
      }

  bool operator==(const QCPColorGradient &other) const
  {
      return ((other.mLevelCount == this->mLevelCount) &&
              (other.mColorInterpolation == this->mColorInterpolation) &&
              (other.mNanHandling == this ->mNanHandling) &&
              (other.mNanColor == this->mNanColor) &&
              (other.mPeriodic == this->mPeriodic) &&
              (other.mColorStops == this->mColorStops));
  }
  bool operator!=(const QCPColorGradient &other) const { return !(*this == other); }

  // getters:
  int levelCount() const { return mLevelCount; }
  QMap<double, QColor> colorStops() const { return mColorStops; }
  ColorInterpolation colorInterpolation() const { return mColorInterpolation; }
  NanHandling nanHandling() const { return mNanHandling; }
  QColor nanColor() const { return mNanColor; }
  bool periodic() const { return mPeriodic; }

  // setters:
  void setLevelCount(int n)
  {
      if (n < 2)
      {
        qDebug() << Q_FUNC_INFO << "n must be greater or equal 2 but was" << n;
        n = 2;
      }
      if (n != mLevelCount)
      {
        mLevelCount = n;
        mColorBufferInvalidated = true;
      }
  }
  void setColorStops(const QMap<double, QColor> &colorStops)
  {
      mColorStops = colorStops;
      mColorBufferInvalidated = true;
  }
  void setColorStopAt(double position, const QColor &color)
  {
      mColorStops.insert(position, color);
      mColorBufferInvalidated = true;
  }
  void setColorInterpolation(ColorInterpolation interpolation)
  {
      if (interpolation != mColorInterpolation)
      {
        mColorInterpolation = interpolation;
        mColorBufferInvalidated = true;
      }
  }
  void setNanHandling(NanHandling handling)
  {
        mNanHandling = handling;
  }
  void setNanColor(const QColor &color)
  {
      mNanColor = color;
  }
  void setPeriodic(bool enabled)
  {
      mPeriodic = enabled;
  }

  // non-property methods:
  void colorize(const double *data, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor=1, bool logarithmic=false)
  {
      // If you change something here, make sure to also adapt color() and the other colorize() overload
      if (!data)
      {
        qDebug() << Q_FUNC_INFO << "null pointer given as data";
        return;
      }
      if (!scanLine)
      {
        qDebug() << Q_FUNC_INFO << "null pointer given as scanLine";
        return;
      }
      if (mColorBufferInvalidated)
        updateColorBuffer();

      const bool skipNanCheck = mNanHandling == nhNone;
      const double posToIndexFactor = !logarithmic ? (mLevelCount-1)/range.size() : (mLevelCount-1)/qLn(range.upper/range.lower);
      for (int i=0; i<n; ++i)
      {
        const double value = data[dataIndexFactor*i];
        if (skipNanCheck || !std::isnan(value))
        {
          int index = int((!logarithmic ? value-range.lower : qLn(value/range.lower)) * posToIndexFactor);
          if (!mPeriodic)
          {
            index = qBound(0, index, mLevelCount-1);
          } else
          {
            index %= mLevelCount;
            if (index < 0)
              index += mLevelCount;
          }
          scanLine[i] = mColorBuffer.at(index);
        } else
        {
          switch(mNanHandling)
          {
          case nhLowestColor: scanLine[i] = mColorBuffer.first(); break;
          case nhHighestColor: scanLine[i] = mColorBuffer.last(); break;
          case nhTransparent: scanLine[i] = qRgba(0, 0, 0, 0); break;
          case nhNanColor: scanLine[i] = mNanColor.rgba(); break;
          case nhNone: break; // shouldn't happen
          }
        }
      }
  }
  void colorize(const double *data, const unsigned char *alpha, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor=1, bool logarithmic=false)
  {
      // If you change something here, make sure to also adapt color() and the other colorize() overload
      if (!data)
      {
        qDebug() << Q_FUNC_INFO << "null pointer given as data";
        return;
      }
      if (!alpha)
      {
        qDebug() << Q_FUNC_INFO << "null pointer given as alpha";
        return;
      }
      if (!scanLine)
      {
        qDebug() << Q_FUNC_INFO << "null pointer given as scanLine";
        return;
      }
      if (mColorBufferInvalidated)
        updateColorBuffer();

      const bool skipNanCheck = mNanHandling == nhNone;
      const double posToIndexFactor = !logarithmic ? (mLevelCount-1)/range.size() : (mLevelCount-1)/qLn(range.upper/range.lower);
      for (int i=0; i<n; ++i)
      {
        const double value = data[dataIndexFactor*i];
        if (skipNanCheck || !std::isnan(value))
        {
          int index = int((!logarithmic ? value-range.lower : qLn(value/range.lower)) * posToIndexFactor);
          if (!mPeriodic)
          {
            index = qBound(0, index, mLevelCount-1);
          } else
          {
            index %= mLevelCount;
            if (index < 0)
              index += mLevelCount;
          }
          if (alpha[dataIndexFactor*i] == 255)
          {
            scanLine[i] = mColorBuffer.at(index);
          } else
          {
            const QRgb rgb = mColorBuffer.at(index);
            const float alphaF = alpha[dataIndexFactor*i]/255.0f;
            scanLine[i] = qRgba(int(qRed(rgb)*alphaF), int(qGreen(rgb)*alphaF), int(qBlue(rgb)*alphaF), int(qAlpha(rgb)*alphaF)); // also multiply r,g,b with alpha, to conform to Format_ARGB32_Premultiplied
          }
        } else
        {
          switch(mNanHandling)
          {
          case nhLowestColor: scanLine[i] = mColorBuffer.first(); break;
          case nhHighestColor: scanLine[i] = mColorBuffer.last(); break;
          case nhTransparent: scanLine[i] = qRgba(0, 0, 0, 0); break;
          case nhNanColor: scanLine[i] = mNanColor.rgba(); break;
          case nhNone: break; // shouldn't happen
          }
        }
      }
  }
  QRgb color(double position, const QCPRange &range, bool logarithmic=false)
  {
      // If you change something here, make sure to also adapt ::colorize()
      if (mColorBufferInvalidated)
        updateColorBuffer();

      const bool skipNanCheck = mNanHandling == nhNone;
      if (!skipNanCheck && std::isnan(position))
      {
        switch(mNanHandling)
        {
        case nhLowestColor: return mColorBuffer.first();
        case nhHighestColor: return mColorBuffer.last();
        case nhTransparent: return qRgba(0, 0, 0, 0);
        case nhNanColor: return mNanColor.rgba();
        case nhNone: return qRgba(0, 0, 0, 0); // shouldn't happen
        }
      }

      const double posToIndexFactor = !logarithmic ? (mLevelCount-1)/range.size() : (mLevelCount-1)/qLn(range.upper/range.lower);
      int index = int((!logarithmic ? position-range.lower : qLn(position/range.lower)) * posToIndexFactor);
      if (!mPeriodic)
      {
        index = qBound(0, index, mLevelCount-1);
      } else
      {
        index %= mLevelCount;
        if (index < 0)
          index += mLevelCount;
      }
      return mColorBuffer.at(index);
  }
  void loadPreset(GradientPreset preset)
  {
      clearColorStops();
      switch (preset)
      {
        case gpGrayscale:
          setColorInterpolation(ciRGB);
          setColorStopAt(0, Qt::black);
          setColorStopAt(1, Qt::white);
          break;
        case gpHot:
          setColorInterpolation(ciRGB);
          setColorStopAt(0, QColor(50, 0, 0));
          setColorStopAt(0.2, QColor(180, 10, 0));
          setColorStopAt(0.4, QColor(245, 50, 0));
          setColorStopAt(0.6, QColor(255, 150, 10));
          setColorStopAt(0.8, QColor(255, 255, 50));
          setColorStopAt(1, QColor(255, 255, 255));
          break;
        case gpCold:
          setColorInterpolation(ciRGB);
          setColorStopAt(0, QColor(0, 0, 50));
          setColorStopAt(0.2, QColor(0, 10, 180));
          setColorStopAt(0.4, QColor(0, 50, 245));
          setColorStopAt(0.6, QColor(10, 150, 255));
          setColorStopAt(0.8, QColor(50, 255, 255));
          setColorStopAt(1, QColor(255, 255, 255));
          break;
        case gpNight:
          setColorInterpolation(ciHSV);
          setColorStopAt(0, QColor(10, 20, 30));
          setColorStopAt(1, QColor(250, 255, 250));
          break;
        case gpCandy:
          setColorInterpolation(ciHSV);
          setColorStopAt(0, QColor(0, 0, 255));
          setColorStopAt(1, QColor(255, 250, 250));
          break;
        case gpGeography:
          setColorInterpolation(ciRGB);
          setColorStopAt(0, QColor(70, 170, 210));
          setColorStopAt(0.20, QColor(90, 160, 180));
          setColorStopAt(0.25, QColor(45, 130, 175));
          setColorStopAt(0.30, QColor(100, 140, 125));
          setColorStopAt(0.5, QColor(100, 140, 100));
          setColorStopAt(0.6, QColor(130, 145, 120));
          setColorStopAt(0.7, QColor(140, 130, 120));
          setColorStopAt(0.9, QColor(180, 190, 190));
          setColorStopAt(1, QColor(210, 210, 230));
          break;
        case gpIon:
          setColorInterpolation(ciHSV);
          setColorStopAt(0, QColor(50, 10, 10));
          setColorStopAt(0.45, QColor(0, 0, 255));
          setColorStopAt(0.8, QColor(0, 255, 255));
          setColorStopAt(1, QColor(0, 255, 0));
          break;
        case gpThermal:
          setColorInterpolation(ciRGB);
          setColorStopAt(0, QColor(0, 0, 50));
          setColorStopAt(0.15, QColor(20, 0, 120));
          setColorStopAt(0.33, QColor(200, 30, 140));
          setColorStopAt(0.6, QColor(255, 100, 0));
          setColorStopAt(0.85, QColor(255, 255, 40));
          setColorStopAt(1, QColor(255, 255, 255));
          break;
        case gpPolar:
          setColorInterpolation(ciRGB);
          setColorStopAt(0, QColor(50, 255, 255));
          setColorStopAt(0.18, QColor(10, 70, 255));
          setColorStopAt(0.28, QColor(10, 10, 190));
          setColorStopAt(0.5, QColor(0, 0, 0));
          setColorStopAt(0.72, QColor(190, 10, 10));
          setColorStopAt(0.82, QColor(255, 70, 10));
          setColorStopAt(1, QColor(255, 255, 50));
          break;
        case gpSpectrum:
          setColorInterpolation(ciHSV);
          setColorStopAt(0, QColor(50, 0, 50));
          setColorStopAt(0.15, QColor(0, 0, 255));
          setColorStopAt(0.35, QColor(0, 255, 255));
          setColorStopAt(0.6, QColor(255, 255, 0));
          setColorStopAt(0.75, QColor(255, 30, 0));
          setColorStopAt(1, QColor(50, 0, 0));
          break;
        case gpJet:
          setColorInterpolation(ciRGB);
          setColorStopAt(0, QColor(0, 0, 100));
          setColorStopAt(0.15, QColor(0, 50, 255));
          setColorStopAt(0.35, QColor(0, 255, 255));
          setColorStopAt(0.65, QColor(255, 255, 0));
          setColorStopAt(0.85, QColor(255, 30, 0));
          setColorStopAt(1, QColor(100, 0, 0));
          break;
        case gpHues:
          setColorInterpolation(ciHSV);
          setColorStopAt(0, QColor(255, 0, 0));
          setColorStopAt(1.0/3.0, QColor(0, 0, 255));
          setColorStopAt(2.0/3.0, QColor(0, 255, 0));
          setColorStopAt(1, QColor(255, 0, 0));
          break;
      }
  }
  void clearColorStops()
  {
      mColorStops.clear();
      mColorBufferInvalidated = true;
  }
  QCPColorGradient inverted() const
  {
      QCPColorGradient result(*this);
      result.clearColorStops();
      for (QMap<double, QColor>::const_iterator it=mColorStops.constBegin(); it!=mColorStops.constEnd(); ++it)
        result.setColorStopAt(1.0-it.key(), it.value());
      return result;
  }

protected:
  // property members:
  int mLevelCount;
  QMap<double, QColor> mColorStops;
  ColorInterpolation mColorInterpolation;
  NanHandling mNanHandling;
  QColor mNanColor;
  bool mPeriodic;

  // non-property members:
  QVector<QRgb> mColorBuffer; // have colors premultiplied with alpha (for usage with QImage::Format_ARGB32_Premultiplied)
  bool mColorBufferInvalidated;

  // non-virtual methods:
  bool stopsUseAlpha() const
  {
      for (QMap<double, QColor>::const_iterator it=mColorStops.constBegin(); it!=mColorStops.constEnd(); ++it)
      {
        if (it.value().alpha() < 255)
          return true;
      }
      return false;
  }
  void updateColorBuffer()
  {
      if (mColorBuffer.size() != mLevelCount)
        mColorBuffer.resize(mLevelCount);
      if (mColorStops.size() > 1)
      {
        double indexToPosFactor = 1.0/double(mLevelCount-1);
        const bool useAlpha = stopsUseAlpha();
        for (int i=0; i<mLevelCount; ++i)
        {
          double position = i*indexToPosFactor;
          QMap<double, QColor>::const_iterator it = mColorStops.lowerBound(position);
          if (it == mColorStops.constEnd()) // position is on or after last stop, use color of last stop
          {
            if (useAlpha)
            {
              const QColor col = std::prev(it).value();
              const double alphaPremultiplier = col.alpha()/255.0; // since we use QImage::Format_ARGB32_Premultiplied
              mColorBuffer[i] = qRgba(int(col.red()*alphaPremultiplier),
                                      int(col.green()*alphaPremultiplier),
                                      int(col.blue()*alphaPremultiplier),
                                      col.alpha());
            } else
              mColorBuffer[i] = std::prev(it).value().rgba();
          } else if (it == mColorStops.constBegin()) // position is on or before first stop, use color of first stop
          {
            if (useAlpha)
            {
              const QColor &col = it.value();
              const double alphaPremultiplier = col.alpha()/255.0; // since we use QImage::Format_ARGB32_Premultiplied
              mColorBuffer[i] = qRgba(int(col.red()*alphaPremultiplier),
                                      int(col.green()*alphaPremultiplier),
                                      int(col.blue()*alphaPremultiplier),
                                      col.alpha());
            } else
              mColorBuffer[i] = it.value().rgba();
          } else // position is in between stops (or on an intermediate stop), interpolate color
          {
            QMap<double, QColor>::const_iterator high = it;
            QMap<double, QColor>::const_iterator low = std::prev(it);
            double t = (position-low.key())/(high.key()-low.key()); // interpolation factor 0..1
            switch (mColorInterpolation)
            {
              case ciRGB:
              {
                if (useAlpha)
                {
                  const int alpha = int((1-t)*low.value().alpha() + t*high.value().alpha());
                  const double alphaPremultiplier = alpha/255.0; // since we use QImage::Format_ARGB32_Premultiplied
                  mColorBuffer[i] = qRgba(int( ((1-t)*low.value().red() + t*high.value().red())*alphaPremultiplier ),
                                          int( ((1-t)*low.value().green() + t*high.value().green())*alphaPremultiplier ),
                                          int( ((1-t)*low.value().blue() + t*high.value().blue())*alphaPremultiplier ),
                                          alpha);
                } else
                {
                  mColorBuffer[i] = qRgb(int( ((1-t)*low.value().red() + t*high.value().red()) ),
                                         int( ((1-t)*low.value().green() + t*high.value().green()) ),
                                         int( ((1-t)*low.value().blue() + t*high.value().blue())) );
                }
                break;
              }
              case ciHSV:
              {
                QColor lowHsv = low.value().toHsv();
                QColor highHsv = high.value().toHsv();
                double hue = 0;
                double hueDiff = highHsv.hueF()-lowHsv.hueF();
                if (hueDiff > 0.5)
                  hue = lowHsv.hueF() - t*(1.0-hueDiff);
                else if (hueDiff < -0.5)
                  hue = lowHsv.hueF() + t*(1.0+hueDiff);
                else
                  hue = lowHsv.hueF() + t*hueDiff;
                if (hue < 0) hue += 1.0;
                else if (hue >= 1.0) hue -= 1.0;
                if (useAlpha)
                {
                  const QRgb rgb = QColor::fromHsvF(hue,
                                                    (1-t)*lowHsv.saturationF() + t*highHsv.saturationF(),
                                                    (1-t)*lowHsv.valueF() + t*highHsv.valueF()).rgb();
                  const double alpha = (1-t)*lowHsv.alphaF() + t*highHsv.alphaF();
                  mColorBuffer[i] = qRgba(int(qRed(rgb)*alpha), int(qGreen(rgb)*alpha), int(qBlue(rgb)*alpha), int(255*alpha));
                }
                else
                {
                  mColorBuffer[i] = QColor::fromHsvF(hue,
                                                     (1-t)*lowHsv.saturationF() + t*highHsv.saturationF(),
                                                     (1-t)*lowHsv.valueF() + t*highHsv.valueF()).rgb();
                }
                break;
              }
            }
          }
        }
      } else if (mColorStops.size() == 1)
      {
        const QRgb rgb = mColorStops.constBegin().value().rgb();
        const double alpha = mColorStops.constBegin().value().alphaF();
        mColorBuffer.fill(qRgba(int(qRed(rgb)*alpha), int(qGreen(rgb)*alpha), int(qBlue(rgb)*alpha), int(255*alpha)));
      } else // mColorStops is empty, fill color buffer with black
      {
        mColorBuffer.fill(qRgb(0, 0, 0));
      }
      mColorBufferInvalidated = false;
  }
};




Q_DECLARE_METATYPE(QCPColorGradient::ColorInterpolation)
Q_DECLARE_METATYPE(QCPColorGradient::NanHandling)
Q_DECLARE_METATYPE(QCPColorGradient::GradientPreset)



#endif // QCPCOLORGRADIENT_H
