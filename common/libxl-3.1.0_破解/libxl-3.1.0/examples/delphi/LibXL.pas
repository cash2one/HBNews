unit LibXL;

{$Z4} 

interface

uses SysUtils;

type
  AlignH = (ALIGNH_GENERAL, ALIGNH_LEFT, ALIGNH_CENTER, ALIGNH_RIGHT, ALIGNH_FILL, ALIGNH_JUSTIFY,
            ALIGNH_MERGE, ALIGNH_DISTRIBUTED);
type
  AlignV = (ALIGNV_TOP, ALIGNV_CENTER, ALIGNV_BOTTOM, ALIGNV_JUSTIFY, ALIGNV_DISTRIBUTED);

type
  BorderStyle = (BORDERSTYLE_NONE, BORDERSTYLE_THIN, BORDERSTYLE_MEDIUM, BORDERSTYLE_DASHED,
                 BORDERSTYLE_DOTTED, BORDERSTYLE_THICK, BORDERSTYLE_DOUBLE, BORDERSTYLE_HAIR,
                 BORDERSTYLE_MEDIUMDASHED, BORDERSTYLE_DASHDOT, BORDERSTYLE_MEDIUMDASHDOT,
                 BORDERSTYLE_DASHDOTDOT, BORDERSTYLE_MEDIUMDASHDOTDOT, BORDERSTYLE_SLANTDASHDOT);
type
  BorderDiagonal = (BORDERDIAGONAL_NONE, BORDERDIAGONAL_DOWN, BORDERDIAGONAL_UP,
                    BORDERDIAGONAL_BOTH);
type
  FillPattern = (FILLPATTERN_NONE, FILLPATTERN_SOLID, FILLPATTERN_GRAY50, FILLPATTERN_GRAY75,
                 FILLPATTERN_GRAY25, FILLPATTEN_HORSTRIPE, FILLPATTERN_VERSTRIPE,
                 FILLPATTERN_REVDIAGSTRIPE, FILLPATTERN_DIAGSTRIPE, FILLPATTERN_DIAGCROSSHATCH,
                 FILLPATTERN_THICKDIAGCROSSHATCH, FILLPATTERN_THINHORSTRIPE,
                 FILLPATTERN_THINVERSTRIPE, FILLPATTERN_THINREVDIAGSTRIPE,
                 FILLPATTERN_THINDIAGSTRIPE, FILLPATTERN_THINHORCROSSHATCH,
                 FILLPATTERN_THINDIAGCROSSHATCH, FILLPATTERN_GRAY12P5, FILLPATTERN_GRAY6P25);
type
  NumFormat = (NUMFORMAT_GENERAL, NUMFORMAT_NUMBER, NUMFORMAT_NUMBER_D2, NUMFORMAT_NUMBER_SEP,
               NUMFORMAT_NUMBER_SEP_D2, NUMFORMAT_CURRENCY_NEGBRA, NUMFORMAT_CURRENCY_NEGBRARED,
               NUMFORMAT_CURRENCY_D2_NEGBRA, NUMFORMAT_CURRENCY_D2_NEGBRARED, NUMFORMAT_PERCENT,
               NUMFORMAT_PERCENT_D2, NUMFORMAT_SCIENTIFIC_D2, NUMFORMAT_FRACTION_ONEDIG,
               NUMFORMAT_FRACTION_TWODIG, NUMFORMAT_DATE, NUMFORMAT_CUSTOM_D_MON_YY,
               NUMFORMAT_CUSTOM_D_MON, NUMFORMAT_CUSTOM_MON_YY, NUMFORMAT_CUSTOM_HMM_AM,
               NUMFORMAT_CUSTOM_HMMSS_AM, NUMFORMAT_CUSTOM_HMM, NUMFORMAT_CUSTOM_HMMSS,
               NUMFORMAT_CUSTOM_MDYYYY_HMM, NUMFORMAT_NUMBER_SEP_NEGBRA = 37,
               NUMFORMAT_NUMBER_SEP_NEGBRARED, NUMFORMAT_NUMBER_D2_SEP_NEGBRA,
               NUMFORMAT_NUMBER_D2_SEP_NEGBRARED, NUMFORMAT_ACCOUNT, NUMFORMAT_ACCOUNTCUR,
               NUMFORMAT_ACCOUNT_D2, NUMFORMAT_ACCOUNT_D2_CUR, NUMFORMAT_CUSTOM_MMSS,
               NUMFORMAT_CUSTOM_H0MMSS, NUMFORMAT_CUSTOM_MMSS0, NUMFORMAT_CUSTOM_000P0E_PLUS0,
               NUMFORMAT_TEXT);
type
  Color = (COLOR_BLACK = 8, COLOR_WHITE, COLOR_RED, COLOR_BRIGHTGREEN, COLOR_BLUE, COLOR_YELLOW,
           COLOR_PINK, COLOR_TURQUOISE, COLOR_DARKRED, COLOR_GREEN, COLOR_DARKBLUE,
           COLOR_DARKYELLOW, COLOR_VIOLET, COLOR_TEAL, COLOR_GRAY25, COLOR_GRAY50,
           COLOR_PERIWINKLE_CF, COLOR_PLUM_CF, COLOR_IVORY_CF, COLOR_LIGHTTURQUOISE_CF,
           COLOR_DARKPURPLE_CF, COLOR_CORAL_CF, COLOR_OCEANBLUE_CF, COLOR_ICEBLUE_CF,
           COLOR_DARKBLUE_CL, COLOR_PINK_CL, COLOR_YELLOW_CL, COLOR_TURQUOISE_CL, COLOR_VIOLET_CL,
           COLOR_DARKRED_CL, COLOR_TEAL_CL, COLOR_BLUE_CL, COLOR_SKYBLUE, COLOR_LIGHTTURQUOISE,
           COLOR_LIGHTGREEN, COLOR_LIGHTYELLOW, COLOR_PALEBLUE, COLOR_ROSE, COLOR_LAVENDER,
           COLOR_TAN, COLOR_LIGHTBLUE, COLOR_AQUA, COLOR_LIME, COLOR_GOLD, COLOR_LIGHTORANGE,
           COLOR_ORANGE, COLOR_BLUEGRAY, COLOR_GRAY40, COLOR_DARKTEAL, COLOR_SEAGREEN,
           COLOR_DARKGREEN, COLOR_OLIVEGREEN, COLOR_BROWN, COLOR_PLUM, COLOR_INDIGO, COLOR_GRAY80,
           COLOR_DEFAULT_FOREGROUND = $0040, COLOR_DEFAULT_BACKGROUND = $0041,
           COLOR_TOOLTIP = $0051, COLOR_AUTO = $7FFF);
type
  CellType = (CELLTYPE_EMPTY, CELLTYPE_NUMBER, CELLTYPE_STRING, CELLTYPE_BOOLEAN, CELLTYPE_BLANK,
              CELLTYPE_ERROR);
type
  ErrorType = (ERRORTYPE_NULL = $00, ERRORTYPE_DIV_0 = $07, ERRORTYPE_VALUE = $0F,
               ERRORTYPE_REF = $17, ERRORTYPE_NAME = $1D, ERRORTYPE_NUM = $24, ERRORTYPE_NA = $2A,
               ERRORTYPE_NOERROR = $FF);
type
  PaneType = (PANETYPE_BOTRIGHT, PANETYPE_TOPRIGHT, PANETYPE_BOTTOMLEFT, PANETYPE_TOPLEFT);
type
  Paper = (PAPER_DEFAULT, PAPER_LETTER, PAPER_LETTERSMALL, PAPER_TABLOID, PAPER_LEDGER, PAPER_LEGAL,
           PAPER_STATEMENT, PAPER_EXECUTIVE, PAPER_A3, PAPER_A4, PAPER_A4SMALL, PAPER_A5, PAPER_B4,
           PAPER_B5, PAPER_FOLIO, PAPER_QUATRO, PAPER_10x14, PAPER_10x17, PAPER_NOTE,
           PAPER_ENVELOPE_9, PAPER_ENVELOPE_10, PAPER_ENVELOPE_11, PAPER_ENVELOPE_12,
           PAPER_ENVELOPE_14, PAPER_C_SIZE, PAPER_D_SIZE, PAPER_E_SIZE, PAPER_ENVELOPE_DL,
           PAPER_ENVELOPE_C5, PAPER_ENVELOPE_C3, PAPER_ENVELOPE_C4, PAPER_ENVELOPE_C6,
           PAPER_ENVELOPE_C65, PAPER_ENVELOPE_B4, PAPER_ENVELOPE_B5, PAPER_ENVELOPE_B6,
           PAPER_ENVELOPE, PAPER_ENVELOPE_MONARCH, PAPER_US_ENVELOPE, PAPER_FANFOLD,
           PAPER_GERMAN_STD_FANFOLD, PAPER_GERMAN_LEGAL_FANFOLD, PAPER_B4_ISO,
           PAPER_JAPANESE_POSTCARD, PAPER_9x11, PAPER_10x11, PAPER_15x11, PAPER_ENVELOPE_INVITE,
           PAPER_US_LETTER_EXTRA = 50, PAPER_US_LEGAL_EXTRA, PAPER_US_TABLOID_EXTRA, PAPER_A4_EXTRA,
           PAPER_LETTER_TRANSVERSE, PAPER_A4_TRANSVERSE, PAPER_LETTER_EXTRA_TRANSVERSE,
           PAPER_SUPERA, PAPER_SUPERB, PAPER_US_LETTER_PLUS, PAPER_A4_PLUS, PAPER_A5_TRANSVERSE,
           PAPER_B5_TRANSVERSE, PAPER_A3_EXTRA, PAPER_A5_EXTRA, PAPER_B5_EXTRA, PAPER_A2,
           PAPER_A3_TRANSVERSE, PAPER_A3_EXTRA_TRANSVERSE, PAPER_JAPANESE_DOUBLE_POSTCARD, PAPER_A6,
           PAPER_JAPANESE_ENVELOPE_KAKU2, PAPER_JAPANESE_ENVELOPE_KAKU3,
           PAPER_JAPANESE_ENVELOPE_CHOU3, PAPER_JAPANESE_ENVELOPE_CHOU4, PAPER_LETTER_ROTATED,
           PAPER_A3_ROTATED, PAPER_A4_ROTATED, PAPER_A5_ROTATED, PAPER_B4_ROTATED, PAPER_B5_ROTATED,
           PAPER_JAPANESE_POSTCARD_ROTATED, PAPER_DOUBLE_JAPANESE_POSTCARD_ROTATED,
           PAPER_A6_ROTATED, PAPER_JAPANESE_ENVELOPE_KAKU2_ROTATED,
           PAPER_JAPANESE_ENVELOPE_KAKU3_ROTATED, PAPER_JAPANESE_ENVELOPE_CHOU3_ROTATED,
           PAPER_JAPANESE_ENVELOPE_CHOU4_ROTATED, PAPER_B6, PAPER_B6_ROTATED, PAPER_12x11,
           PAPER_JAPANESE_ENVELOPE_YOU4, PAPER_JAPANESE_ENVELOPE_YOU4_ROTATED, PAPER_PRC16K,
           PAPER_PRC32K, PAPER_PRC32K_BIG, PAPER_PRC_ENVELOPE1, PAPER_PRC_ENVELOPE2,
           PAPER_PRC_ENVELOPE3, PAPER_PRC_ENVELOPE4, PAPER_PRC_ENVELOPE5, PAPER_PRC_ENVELOPE6,
           PAPER_PRC_ENVELOPE7, PAPER_PRC_ENVELOPE8, PAPER_PRC_ENVELOPE9, PAPER_PRC_ENVELOPE10,
           PAPER_PRC16K_ROTATED, PAPER_PRC32K_ROTATED, PAPER_PRC32KBIG_ROTATED,
           PAPER_PRC_ENVELOPE1_ROTATED, PAPER_PRC_ENVELOPE2_ROTATED, PAPER_PRC_ENVELOPE3_ROTATED,
           PAPER_PRC_ENVELOPE4_ROTATED, PAPER_PRC_ENVELOPE5_ROTATED, PAPER_PRC_ENVELOPE6_ROTATED,
           PAPER_PRC_ENVELOPE7_ROTATED, PAPER_PRC_ENVELOPE8_ROTATED, PAPER_PRC_ENVELOPE9_ROTATED,
           PAPER_PRC_ENVELOPE10_ROTATED);
type
  Underline = (UNDERLINE_NONE, UNDERLINE_SINGLE, UNDERLINE_DOUBLE,
               UNDERLINE_SINGLEACC = $21, UNDERLINE_DOUBLEACC = $22);
type
  Script = (SCRIPT_NORMAL, SCRIPT_SUPER, SCRIPT_SUB);
type
  BookHandle = Pointer;
type
  SheetHandle = Pointer;
type
  FormatHandle = Pointer;
type
  FontHandle = Pointer;
type
  PFormatHandle = ^FormatHandle;
type
  ByteArray = array of byte;
type
  PByteArray = ^ByteArray;
type
  PCardinal = ^Cardinal;
type
  PInteger = ^Integer;

////////////////////////////////////////////////////////////////////////////////////////////////////

function xlCreateBookC(): BookHandle cdecl;
external 'libxl' name 'xlCreateBookCA';

function xlCreateXMLBookC(): BookHandle cdecl;
external 'libxl' name 'xlCreateXMLBookCA';

function xlBookLoad(handle: BookHandle; filename: PAnsiChar): Integer cdecl;
external 'libxl' name 'xlBookLoadA';

function xlBookSave(handle: BookHandle; filename: PAnsiChar): Integer cdecl;
external 'libxl' name 'xlBookSaveA';

function xlBookLoadRaw(handle: BookHandle; data: PByteArray; size: Cardinal): Integer cdecl;
external 'libxl' name 'xlBookLoadRawA';

function xlBookSaveRaw(handle: BookHandle; data: PByteArray; size: PCardinal): Integer cdecl;
external 'libxl' name 'xlBookSaveRawA';

function xlBookAddSheet(handle: BookHandle; name: PAnsiChar; initSheet: SheetHandle): SheetHandle cdecl;
external 'libxl' name 'xlBookAddSheetA';

function xlBookGetSheet(handle: BookHandle; index: Integer): SheetHandle cdecl;
external 'libxl' name 'xlBookGetSheetA';

function xlBookDelSheet(handle: BookHandle; index: Integer): Integer cdecl;
external 'libxl' name 'xlBookDelSheetA';

function xlBookSheetCount(handle: BookHandle): Integer cdecl;
external 'libxl' name 'xlBookSheetCountA';

function xlBookAddFormat(handle: BookHandle; initFormat: FormatHandle): FormatHandle cdecl;
external 'libxl' name 'xlBookAddFormatA';

function xlBookAddFont(handle: BookHandle; initFont: FormatHandle): FontHandle cdecl;
external 'libxl' name 'xlBookAddFontA';

function xlBookAddCustomNumFormat(handle: BookHandle; const customNumFormat: PAnsiChar): Integer cdecl;
external 'libxl' name 'xlBookAddCustomNumFormatA';

function xlBookCustomNumFormat(handle: BookHandle; fmt: Integer): PAnsiChar cdecl;
external 'libxl' name 'xlBookCustomNumFormatA';

function xlBookFormat(handle: BookHandle; index: Integer): FormatHandle cdecl;
external 'libxl' name 'xlBookFormatA';

function xlBookFormatSize(handle: BookHandle): Integer cdecl;
external 'libxl' name 'xlBookFormatSizeA';

function xlBookFont(handle: BookHandle; index: Integer): FontHandle cdecl;
external 'libxl' name 'xlBookFontA';

function xlBookFontSize(handle: BookHandle): Integer cdecl;
external 'libxl' name 'xlBookFontSizeA';

function xlBookDatePack(handle: BookHandle; year: Integer; month: Integer; day: Integer; hour: Integer; min: Integer; sec: Integer; msec: Integer): double cdecl;
external 'libxl' name 'xlBookDatePackA';

function xlBookDateUnpack(handle: BookHandle; value: double; year: PInteger; month: PInteger; day: PInteger; hour: PInteger; min: PInteger; sec: PInteger; msec: PInteger): Integer cdecl;
external 'libxl' name 'xlBookDateUnpackA';

function xlBookColorPack(handle: BookHandle; red: Integer; green: Integer; blue: Integer): Integer cdecl;
external 'libxl' name 'xlBookColorPackA';

procedure xlBookColorUnpack(handle: BookHandle; color: Integer; red: PInteger; green: PInteger; blue: PInteger) cdecl;
external 'libxl' name 'xlBookColorUnpackA';

function xlBookActiveSheet(handle: BookHandle): Integer cdecl;
external 'libxl' name 'xlBookActiveSheetA';

procedure xlBookSetActiveSheet(handle: BookHandle; index: Integer) cdecl;
external 'libxl' name 'xlBookSetActiveSheetA';

function xlBookAddPicture(handle: BookHandle; filename: PAnsiChar): Integer cdecl;
external 'libxl' name 'xlBookAddPictureA';

function xlBookDefaultFont(handle: BookHandle; fontSize: PInteger): PAnsiChar cdecl;
external 'libxl' name 'xlBookDefaultFontA';

procedure xlBookSetDefaultFont(handle: BookHandle; const name: PAnsiChar; fontSize: Integer) cdecl;
external 'libxl' name 'xlBookSetDefaultFontA';

procedure xlBookSetKey(handle: BookHandle; const name: PWideChar; const key: PWideChar) cdecl;
external 'libxl' name 'xlBookSetKeyA';

function xlBookRgbMode(handle: BookHandle): Integer cdecl;
external 'libxl' name 'xlBookRgbModeA';

procedure xlBookSetRgbMode(handle: BookHandle; rgbMode: Integer) cdecl;
external 'libxl' name 'xlBookSetRgbModeA';

procedure xlBookSetLocale(handle: BookHandle; const locale: PAnsiChar) cdecl;
external 'libxl' name 'xlBookSetLocaleA';

function xlBookErrorMessage(handle: BookHandle): PAnsiChar cdecl;
external 'libxl' name 'xlBookErrorMessageA';

procedure xlBookRelease(handle: BookHandle) cdecl;
external 'libxl' name 'xlBookReleaseA';

////////////////////////////////////////////////////////////////////////////////////////////////////

procedure xlSheetSetName(handle: SheetHandle; const name: PAnsiChar) cdecl;
external 'libxl' name 'xlSheetSetNameA';

function xlSheetCopyCell(handle: SheetHandle; rowSrc, colSrc, rowDst, colDst: Integer): Integer cdecl;
external 'libxl' name 'xlSheetCopyCellA';

function xlSheetCellType(handle: SheetHandle; row: Integer; col: Integer): Integer cdecl;
external 'libxl' name 'xlSheetCellTypeA';

function xlSheetReadStr(handle: SheetHandle; row: Integer; col: Integer; format: PFormatHandle): PAnsiChar cdecl;
external 'libxl' name 'xlSheetReadStrA';

function xlSheetWriteStr(handle: SheetHandle; row: Integer; col: Integer; value: PAnsiChar; format: FormatHandle): Integer cdecl;
external 'libxl' name 'xlSheetWriteStrA';

function xlSheetReadNum(handle: SheetHandle; row: Integer; col: Integer; format: PFormatHandle): double cdecl;
external 'libxl' name 'xlSheetReadNumA';

function xlSheetWriteNum(handle: SheetHandle; row: Integer; col: Integer; value: double; format: FormatHandle): Integer cdecl;
external 'libxl' name 'xlSheetWriteNumA';

function xlSheetReadBool(handle: SheetHandle; row: Integer; col: Integer; format: PFormatHandle): Integer cdecl;
external 'libxl' name 'xlSheetReadBoolA';

function xlSheetWriteBool(handle: SheetHandle; row: Integer; col: Integer; value: Integer; format: FormatHandle): Integer cdecl;
external 'libxl' name 'xlSheetWriteBoolA';

function xlSheetReadBlank(handle: SheetHandle; row: Integer; col: Integer; format: PFormatHandle): Integer cdecl;
external 'libxl' name 'xlSheetReadBlankA';

function xlSheetWriteBlank(handle: SheetHandle; row: Integer; col: Integer; format: FormatHandle): Integer cdecl;
external 'libxl' name 'xlSheetWriteBlankA';

function xlSheetReadComment(handle: SheetHandle; row: Integer; col: Integer): PAnsiChar cdecl;
external 'libxl' name 'xlSheetReadCommentA';

procedure xlSheetWriteComment(handle: SheetHandle; row: Integer; col: Integer; const value: PAnsiChar; const author: PAnsiChar; width: Integer; height: Integer) cdecl;
external 'libxl' name 'xlSheetWriteCommentA';

function xlSheetIsDate(handle: SheetHandle; row: Integer; col: Integer): Integer cdecl;
external 'libxl' name 'xlSheetIsDateA';

function xlSheetReadError(handle: SheetHandle; row: Integer; col: Integer): Integer cdecl;
external 'libxl' name 'xlSheetReadErrorA';

function xlSheetColWidth(handle: SheetHandle; col: Integer): double cdecl;
external 'libxl' name 'xlSheetColWidthA';

function xlSheetRowHeight(handle: SheetHandle; row: Integer): double cdecl;
external 'libxl' name 'xlSheetRowHeightA';

function xlSheetSetCol(handle: SheetHandle; colFirst: Integer; colLast: Integer; width: double; format: FormatHandle; hidden: Integer): Integer cdecl;
external 'libxl' name 'xlSheetSetColA';

function xlSheetSetRow(handle: SheetHandle; row: Integer; height: double; format: FormatHandle; hidden: Integer): Integer cdecl;
external 'libxl' name 'xlSheetSetRowA';

function xlSheetGetMerge(handle: SheetHandle; row: Integer; col: Integer; rowFirst: PInteger; rowLast: PInteger; colFirst: PInteger; colLast: PInteger) : Integer cdecl;
external 'libxl' name 'xlSheetGetMergeA';

function xlSheetSetMerge(handle: SheetHandle; rowFirst: Integer; rowLast: Integer; colFirst: Integer; colLast: Integer): Integer cdecl;
external 'libxl' name 'xlSheetSetMergeA';

function xlSheetDelMerge(handle: SheetHandle; row: Integer; col: Integer): Integer cdecl;
external 'libxl' name 'xlSheetDelMergeA';

procedure xlSheetSetPicture(handle: SheetHandle; row: Integer; col: Integer; pictureId: Integer; scale: double) cdecl;
external 'libxl' name 'xlSheetSetPictureA';

procedure xlSheetSetPicture2(handle: SheetHandle; row: Integer; col: Integer; pictureId: Integer; widht: Integer; height: Integer) cdecl;
external 'libxl' name 'xlSheetSetPicture2A';

function xlSheetSetHorPageBreak(handle: SheetHandle; row: Integer; pageBreak: Integer): Integer cdecl;
external 'libxl' name 'xlSheetSetHorPageBreakA';

function xlSheetSetVerPageBreak(handle: SheetHandle; col: Integer; pageBreak: Integer): Integer cdecl;
external 'libxl' name 'xlSheetSetVerPageBreakA';

procedure xlSheetSplit(handle: SheetHandle; row: Integer; col: Integer) cdecl;
external 'libxl' name 'xlSheetSplitA';

function xlSheetGroupRows(handle: SheetHandle; rowFirst: Integer; rowLast: Integer; collapsed: Integer): Integer cdecl;
external 'libxl' name 'xlSheetGroupRowsA';

function xlSheetGroupCols(handle: SheetHandle; colFirst: Integer; colLast: Integer; collapsed: Integer): Integer cdecl;
external 'libxl' name 'xlSheetGroupColsA';

function xlSheetGroupSummaryBelow(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetGroupSummaryBelowA';

procedure xlSheetSetGroupSummaryBelow(handle: SheetHandle; below: Integer) cdecl;
external 'libxl' name 'xlSheetSetGroupSummaryBelowA';

function xlSheetGroupSummaryRight(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetGroupSummaryRightA';

procedure xlSheetSetGroupSummaryRight(handle: SheetHandle; right: Integer) cdecl;
external 'libxl' name 'xlSheetSetGroupSummaryRightA';

procedure xlSheetClear(handle: SheetHandle; rowFirst: Integer; rowLast: Integer; colFirst: Integer; colLast: Integer) cdecl;
external 'libxl' name 'xlSheetClearA';

function xlSheetInsertCol(handle: SheetHandle; colFirst: Integer; colLast: Integer): Integer cdecl;
external 'libxl' name 'xlSheetInsertColA';

function xlSheetInsertRow(handle: SheetHandle; rowFirst: Integer; rowLast: Integer): Integer cdecl;
external 'libxl' name 'xlSheetInsertRowA';

function xlSheetRemoveCol(handle: SheetHandle; colFirst: Integer; colLast: Integer): Integer cdecl;
external 'libxl' name 'xlSheetRemoveColA';

function xlSheetRemoveRow(handle: SheetHandle; rowFirst: Integer; rowLast: Integer): Integer cdecl;
external 'libxl' name 'xlSheetRemoveRowA';

function xlSheetFirstRow(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetFirstRowA';

function xlSheetLastRow(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetLastRowA';

function xlSheetFirstCol(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetFirstColA';

function xlSheetLastCol(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetLastColA';

function xlSheetDisplayGridlines(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetDisplayGridlinesA';

procedure xlSheetSetDisplayGridlines(handle: SheetHandle; show: Integer) cdecl;
external 'libxl' name 'xlSheetSetDisplayGridlinesA';

function xlSheetPrintGridlines(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetPrintGridlinesA';

procedure xlSheetSetPrintGridlines(handle: SheetHandle; print: Integer) cdecl;
external 'libxl' name 'xlSheetSetPrintGridlinesA';

function xlSheetZoom(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetZoomA';

procedure xlSheetSetZoom(handle: SheetHandle; print: Integer) cdecl;
external 'libxl' name 'xlSheetSetZoomA';

function xlSheetPrintZoom(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetPrintZoomA';

procedure xlSheetSetPrintZoom(handle: SheetHandle; print: Integer) cdecl;
external 'libxl' name 'xlSheetSetPrintZoomA';

function xlSheetLandscape(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetLandscapeA';

procedure xlSheetSetLandscape(handle: SheetHandle; landscape: Integer) cdecl;
external 'libxl' name 'xlSheetSetLandscapeA';

function xlSheetPaper(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetPaperA';

procedure xlSheetSetPaper(handle: SheetHandle; Paper: Integer) cdecl;
external 'libxl' name 'xlSheetSetPaperA';

function xlSheetHeader(handle: SheetHandle): PAnsiChar cdecl;
external 'libxl' name 'xlSheetHeaderA';

function xlSheetSetHeader(handle: SheetHandle; const header: PAnsiChar; margin: double): Integer cdecl;
external 'libxl' name 'xlSheetSetHeaderA';

function xlSheetHeaderMargin(handle: SheetHandle): double cdecl;
external 'libxl' name 'xlSheetHeaderMarginA';

function xlSheetFooter(handle: SheetHandle): PAnsiChar cdecl;
external 'libxl' name 'xlSheetFooterA';

function xlSheetSetFooter(handle: SheetHandle; const footer: PAnsiChar; margin: double): Integer cdecl;
external 'libxl' name 'xlSheetSetFooterA';

function xlSheetFooterMargin(handle: SheetHandle): double cdecl;
external 'libxl' name 'xlSheetFooterMarginA';

function xlSheetHCenter(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetHCenterA';

procedure xlSheetSetHCenter(handle: SheetHandle; hCenter: Integer) cdecl;
external 'libxl' name 'xlSheetSetHCenterA';

function xlSheetVCenter(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetVCenterA';

procedure xlSheetSetVCenter(handle: SheetHandle; vCenter: Integer) cdecl;
external 'libxl' name 'xlSheetSetVCenterA';

function xlSheetMarginLeft(handle: SheetHandle): double cdecl;
external 'libxl' name 'xlSheetMarginLeftA';

procedure xlSheetSetMarginLeft(handle: SheetHandle; margin: double) cdecl;
external 'libxl' name 'xlSheetSetMarginLeftA';

function xlSheetMarginRight(handle: SheetHandle): double cdecl;
external 'libxl' name 'xlSheetMarginRightA';

procedure xlSheetSetMarginRight(handle: SheetHandle; margin: double) cdecl;
external 'libxl' name 'xlSheetSetMarginRightA';

function xlSheetMarginTop(handle: SheetHandle): double cdecl;
external 'libxl' name 'xlSheetMarginTopA';

procedure xlSheetSetMarginTop(handle: SheetHandle; margin: double) cdecl;
external 'libxl' name 'xlSheetSetMarginTopA';

function xlSheetMarginBottom(handle: SheetHandle): double cdecl;
external 'libxl' name 'xlSheetMarginBottomA';

procedure xlSheetSetMarginBottom(handle: SheetHandle; margin: double) cdecl;
external 'libxl' name 'xlSheetSetMarginBottomA';

function xlSheetPrintRowCol(handle: SheetHandle): Integer cdecl;
external 'libxl' name 'xlSheetPrintRowColA';

procedure xlSheetSetPrintRowCol(handle: SheetHandle; print: Integer) cdecl;
external 'libxl' name 'xlSheetSetPrintRowColA';

procedure xlSheetSetPrintRepeatRows(handle: SheetHandle; rowFirst: Integer; rowLast: Integer) cdecl;
external 'libxl' name 'xlSheetSetPrintRepeatRowsA';

procedure xlSheetSetPrintRepeatCols(handle: SheetHandle; colFirst: Integer; colLast: Integer) cdecl;
external 'libxl' name 'xlSheetSetPrintRepeatColsA';

procedure xlSheetSetPrintArea(handle: SheetHandle; rowFirst: Integer; rowLast: Integer; colFirst: Integer; colLast: Integer) cdecl;
external 'libxl' name 'xlSheetSetPrintAreaA';

procedure xlSheetClearPrintRepeats(handle: SheetHandle) cdecl;
external 'libxl' name 'xlSheetClearPrintRepeatsA';

procedure xlSheetClearPrintArea(handle: SheetHandle) cdecl;
external 'libxl' name 'xlSheetClearPrintAreaA';

function xlSheetSetNamedRange(handle: SheetHandle; const name: PAnsiChar; rowFirst: Integer; rowLast: Integer; colFirst: Integer; colLast: Integer): Integer cdecl;
external 'libxl' name 'xlSheetSetNamedRangeA';

function xlSheetDelNamedRange(handle: SheetHandle; const name: PAnsiChar): Integer cdecl;
external 'libxl' name 'xlSheetDelNamedRangeA';

function xlSheetName(handle: SheetHandle): PAnsiChar cdecl;
external 'libxl' name 'xlSheetNameA';

function xlSheetCellFormat(handle: SheetHandle; row: Integer; col: Integer): FormatHandle cdecl;
external 'libxl' name 'xlSheetNameA';

function xlSheetIsFormula(handle: SheetHandle; row: Integer; col: Integer): Integer cdecl;
external 'libxl' name 'xlSheetIsFormulaA';

function xlSheetReadFormula(handle: SheetHandle; row: Integer; col: Integer; format: PFormatHandle): PAnsiChar cdecl;
external 'libxl' name 'xlSheetReadFormulaA';

function xlSheetWriteFormula(handle: SheetHandle; row: Integer; col: Integer; value: PAnsiChar; format: FormatHandle): Integer cdecl;
external 'libxl' name 'xlSheetWriteFormulaA';

////////////////////////////////////////////////////////////////////////////////////////////////////

function xlFormatFont(handle: FormatHandle): FontHandle cdecl;
external 'libxl' name 'xlFormatFontA';

function xlFormatSetFont(handle: FormatHandle; FontHandle: FontHandle): FontHandle cdecl;
external 'libxl' name 'xlFormatSetFontA';

function xlFormatNumFormat(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatNumFormatA';

procedure xlFormatSetNumFormat(handle: FormatHandle; NumFormat: Integer) cdecl;
external 'libxl' name 'xlFormatSetNumFormatA';

function xlFormatAlignH(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatAlignHA';

procedure xlFormatSetAlignH(handle: FormatHandle; align: Integer) cdecl;
external 'libxl' name 'xlFormatSetAlignHA';

function xlFormatAlignV(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatAlignVA';

procedure xlFormatSetAlignV(handle: FormatHandle; align: Integer) cdecl;
external 'libxl' name 'xlFormatSetAlignVA';

function xlFormatWrap(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatWrapA';

procedure xlFormatSetWrap(handle: FormatHandle; wrap: Integer) cdecl;
external 'libxl' name 'xlFormatSetWrapA';

function xlFormatRotation(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatRotationA';

function xlFormatSetRotation(handle: FormatHandle; rotation: Integer): Integer cdecl;
external 'libxl' name 'xlFormatSetRotationA';

function xlFormatIndent(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatIndentA';

procedure xlFormatSetIndent(handle: FormatHandle; indent: Integer) cdecl;
external 'libxl' name 'xlFormatSetIndentA';

function xlFormatShrinkToFit(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatShrinkToFitA';

procedure xlFormatSetShrinkToFit(handle: FormatHandle; shrinkToFit: Integer) cdecl;
external 'libxl' name 'xlFormatSetShrinkToFitA';

procedure xlFormatSetBorder(handle: FormatHandle; style: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderA';

procedure xlFormatSetBorderColor(handle: FormatHandle; Color: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderColorA';

function xlFormatBorderLeft(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderLeftA';

procedure xlFormatSetBorderLeft(handle: FormatHandle; style: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderLeftA';

function xlFormatBorderRight(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderRightA';

procedure xlFormatSetBorderRight(handle: FormatHandle; style: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderRightA';

function xlFormatBorderTop(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderTopA';

procedure xlFormatSetBorderTop(handle: FormatHandle; style: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderTopA';

function xlFormatBorderBottom(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderBottomA';

procedure xlFormatSetBorderBottom(handle: FormatHandle; style: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderBottomA';

function xlFormatBorderLeftColor(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderLeftColorA';

procedure xlFormatSetBorderLeftColor(handle: FormatHandle; Color: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderLeftColorA';

function xlFormatBorderRightColor(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderRightColorA';

procedure xlFormatSetBorderRightColor(handle: FormatHandle; Color: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderRightColorA';

function xlFormatBorderTopColor(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderTopColorA';

procedure xlFormatSetBorderTopColor(handle: FormatHandle; Color: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderTopColorA';

function xlFormatBorderBottomColor(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderBottomColorA';

procedure xlFormatSetBorderBottomColor(handle: FormatHandle; Color: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderBottomColorA';

function xlFormatBorderDiagonal(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderDiagonalA';

procedure xlFormatSetBorderDiagonal(handle: FormatHandle; border: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderDiagonalA';

function xlFormatBorderDiagonalStyle(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderDiagonalStyleA';

procedure xlFormatSetBorderDiagonalStyle(handle: FormatHandle; style: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderDiagonalStyleA';

function xlFormatBorderDiagonalColor(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatBorderDiagonalColorA';

procedure xlFormatSetBorderDiagonalColor(handle: FormatHandle; Color: Integer) cdecl;
external 'libxl' name 'xlFormatSetBorderDiagonalColorA';

function xlFormatFillPattern(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatFillPatternA';

procedure xlFormatSetFillPattern(handle: FormatHandle; pattern: Integer) cdecl;
external 'libxl' name 'xlFormatSetFillPatternA';

function xlFormatPatternForegroundColor(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatPatternForegroundColorA';

procedure xlFormatSetPatternForegroundColor(handle: FormatHandle; Color: Integer) cdecl;
external 'libxl' name 'xlFormatSetPatternForegroundColorA';

function xlFormatPatternBackgroundColor(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatPatternBackgroundColorA';

procedure xlFormatSetPatternBackgroundColor(handle: FormatHandle; Color: Integer) cdecl;
external 'libxl' name 'xlFormatSetPatternBackgroundColorA';

function xlFormatLocked(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatLockedA';

procedure xlFormatSetLocked(handle: FormatHandle; locked: Integer) cdecl;
external 'libxl' name 'xlFormatSetLockedA';

function xlFormatHidden(handle: FormatHandle): Integer cdecl;
external 'libxl' name 'xlFormatHiddenA';

procedure xlFormatSetHidden(handle: FormatHandle; hidden: Integer) cdecl;
external 'libxl' name 'xlFormatSetHiddenA';

////////////////////////////////////////////////////////////////////////////////////////////////////

function xlFontSize(handle: FontHandle): Integer cdecl;
external 'libxl' name 'xlFontSizeA';

procedure xlFontSetSize(handle: FontHandle; size: Integer) cdecl;
external 'libxl' name 'xlFontSetSizeA';

function xlFontItalic(handle: FontHandle): Integer cdecl;
external 'libxl' name 'xlFontItalicA';

procedure xlFontSetItalic(handle: FontHandle; italic: Integer) cdecl;
external 'libxl' name 'xlFontSetItalicA';

function xlFontStrikeOut(handle: FontHandle): Integer cdecl;
external 'libxl' name 'xlFontStrikeOutA';

procedure xlFontSetStrikeOut(handle: FontHandle; strikeOut: Integer) cdecl;
external 'libxl' name 'xlFontSetStrikeOutA';

function xlFontColor(handle: FontHandle): Integer cdecl;
external 'libxl' name 'xlFontColorA';

procedure xlFontSetColor(handle: FontHandle; Color: Integer) cdecl;
external 'libxl' name 'xlFontSetColorA';

function xlFontBold(handle: FontHandle): Integer cdecl;
external 'libxl' name 'xlFontBoldA';

procedure xlFontSetBold(handle: FontHandle; bold: Integer) cdecl;
external 'libxl' name 'xlFontSetBoldA';

function xlFontScript(handle: FontHandle): Integer cdecl;
external 'libxl' name 'xlFontScriptA';

procedure xlFontSetScript(handle: FontHandle; Script: Integer) cdecl;
external 'libxl' name 'xlFontSetScriptA';

function xlFontUnderline(handle: FontHandle): Integer cdecl;
external 'libxl' name 'xlFontUnderlineA';

procedure xlFontSetUnderline(handle: FontHandle; Underline: Integer) cdecl;
external 'libxl' name 'xlFontSetUnderlineA';

function xlFontName(handle: FontHandle): PAnsiChar cdecl;
external 'libxl' name 'xlFontNameA';

function xlFontSetName(handle: FontHandle; const name: PAnsiChar): Integer cdecl;
external 'libxl' name 'xlFontSetNameA';

////////////////////////////////////////////////////////////////////////////////////////////////////

type
  ExceptionXL = class(Exception)
  private
  protected
  public
  published
    constructor Create(book: BookHandle); overload;
  end;

type
  TFont = class(TObject)
  private
    book: BookHandle;

    procedure Setsize(const value: Integer);
    function Getsize: Integer;

    procedure Setbold(const value: boolean);
    function Getbold: boolean;

    procedure Setitalic(const value: boolean);
    function Getitalic: boolean;

    procedure SetUnderline(const value: Underline);
    function GetUnderline: Underline;

    procedure SetstrikeOut(const value: boolean);
    function GetstrikeOut: boolean;

    procedure Setscript(const value: Script);
    function Getscript: Script;

    procedure Setcolor(const value: Color);
    function Getcolor: Color;

    procedure SetName(const value: AnsiString);
    function GetName: AnsiString;

    function Error: AnsiString;

  protected
  public
    handle: FontHandle;

    property size: Integer read Getsize write Setsize;
    property bold: boolean read Getbold write Setbold;
    property italic: boolean read Getitalic write Setitalic;
    property Underline: Underline read GetUnderline write SetUnderline;
    property strikeOut: boolean read GetstrikeOut write SetstrikeOut;
    property Script: Script read Getscript write Setscript;
    property Color: Color read Getcolor write Setcolor;
    property name: AnsiString read GetName write SetName;

  published
    constructor Create(handle: FontHandle; book: BookHandle);
    destructor Destroy; override;
  end;

type
  TFormat = class(TObject)
  private
    book: BookHandle;
    procedure Error;

    procedure Setfont(const value: TFont);
    function Getfont: TFont;

    procedure SetalignH(const value: AlignH);
    function GetalignH: AlignH;

    procedure SetAlignV(const value: AlignV);
    function GetAlignV: AlignV;
    procedure Setwrap(const value: boolean);
    function Getwrap: boolean;
    procedure Setrotation(const value: Integer);
    function Getrotation: Integer;
    procedure Setindent(const value: Integer);
    function Getindent: Integer;
    procedure SetshrinkToFit(const value: boolean);
    function GetshrinkToFit: boolean;

    procedure SetborderLeft(const value: BorderStyle);
    function GetborderLeft: BorderStyle;
    procedure SetborderRight(const value: BorderStyle);
    function GetborderRight: BorderStyle;
    procedure SetborderTop(const value: BorderStyle);
    function GetborderTop: BorderStyle;
    procedure SetborderBottom(const value: BorderStyle);
    function GetborderBottom: BorderStyle;

    procedure SetborderLeftColor(const value: Color);
    function GetborderLeftColor: Color;
    procedure SetborderRightColor(const value: Color);
    function GetborderRightColor: Color;
    procedure SetborderTopColor(const value: Color);
    function GetborderTopColor: Color;
    procedure SetborderBottomColor(const value: Color);
    function GetborderBottomColor: Color;

    procedure SetborderDiagonal(const value: BorderDiagonal);
    function GetborderDiagonal: BorderDiagonal;
    procedure SetborderDiagonalColor(const value: Color);
    function GetborderDiagonalColor: Color;

    procedure SetfillPattern(const value: FillPattern);
    function GetfillPattern: FillPattern;

    procedure SetpatternForegroundColor(const value: Color);
    function GetpatternForegroundColor: Color;
    procedure SetpatternBackgroundColor(const value: Color);
    function GetpatternBackgroundColor: Color;

    procedure Setlocked(const value: boolean);
    function Getlocked: boolean;

    procedure Sethidden(const value: boolean);
    function Gethidden: boolean;

  protected
  public
    handle: FormatHandle;

    function NumFormat: Integer;
    procedure setNumFormat(NumFormat: Integer); overload;
    procedure setNumFormat(value: NumFormat); overload;
    procedure setBorder(style: BorderStyle);
    procedure setBorderColor(value: Color);

    property hidden: boolean read Gethidden write Sethidden;
    property locked: boolean read Getlocked write Setlocked;
    property patternBackgroundColor: Color read GetpatternBackgroundColor write SetpatternBackgroundColor;
    property FillPattern: FillPattern read GetfillPattern write SetfillPattern;
    property patternForegroundColor: Color read GetpatternForegroundColor write SetpatternForegroundColor;
    property BorderDiagonal: BorderDiagonal read GetborderDiagonal write SetborderDiagonal;
    property borderDiagonalColor: Color read GetborderDiagonalColor write SetborderDiagonalColor;

    property font: TFont read Getfont write Setfont;
    property AlignH: AlignH read GetalignH write SetalignH;
    property AlignV: AlignV read GetAlignV write SetAlignV;
    property wrap: boolean read Getwrap write Setwrap;
    property rotation: Integer read Getrotation write Setrotation;
    property indent: Integer read Getindent write Setindent;
    property shrinkToFit: boolean read GetshrinkToFit write SetshrinkToFit;

    property borderLeft: BorderStyle read GetborderLeft write SetborderLeft;
    property borderRight: BorderStyle read GetborderRight write SetborderRight;
    property borderTop: BorderStyle read GetborderTop write SetborderTop;
    property borderBottom: BorderStyle read GetborderBottom write SetborderBottom;

    property borderLeftColor: Color read GetborderLeftColor write SetborderLeftColor;
    property borderRightColor: Color read GetborderRightColor write SetborderRightColor;
    property borderTopColor: Color read GetborderTopColor write SetborderTopColor;
    property borderBottomColor: Color read GetborderBottomColor write SetborderBottomColor;

  published
    constructor Create(handle: FormatHandle; book: BookHandle);
    destructor Destroy; override;
  end;

type
  TSheet = class(TObject)
  private
    handle: SheetHandle;
    book: BookHandle;
    procedure Error;

    procedure SetDisplayGridlines(const value: boolean);
    function GetDisplayGridlines: boolean;

    procedure SetprintGridlines(const value: boolean);
    function GetprintGridlines: boolean;

    procedure SetZoom(const value: Integer);
    function GetZoom: Integer;

    procedure SetprintZoom(const value: Integer);
    function GetprintZoom: Integer;

    procedure Setlandscape(const value: boolean);
    function Getlandscape: boolean;

    procedure SetPaper(const value: Paper);
    function GetPaper: Paper;

    procedure Setheader(const value: AnsiString);
    function Getheader: AnsiString;

    procedure SetheaderMargin(const value: double);
    function GetheaderMargin: double;

    function Getfooter: AnsiString;
    procedure Setfooter(const value: AnsiString);
    function GetfooterMargin: double;
    procedure SetfooterMargin(const value: double);

    function GethCenter: boolean;
    procedure SethCenter(const value: boolean);

    function GetvCenter: boolean;
    procedure SetvCenter(const value: boolean);

    function GetmarginLeft: double;
    procedure SetmarginLeft(const value: double);
    function GetmarginRight: double;
    procedure SetmarginRight(const value: double);
    function GetmarginBottom: double;
    function GetmarginTop: double;
    procedure SetmarginBottom(const value: double);
    procedure SetmarginTop(const value: double);

    function GetprintRowCol: boolean;
    procedure SetprintRowCol(const value: boolean);

    function GetName: AnsiString;
    procedure SetName(const value: AnsiString);

    function GetGroupSummaryBelow: boolean;
    procedure SetGroupSummaryBelow(const value: boolean);

    function GetGroupSummaryRight: boolean;
    procedure SetGroupSummaryRight(const value: boolean);

  protected
  public

    property displayGridlines: boolean read GetDisplayGridlines write SetDisplayGridlines;
    property printGridlines: boolean read GetprintGridlines write SetprintGridlines;
    property zoom: Integer read GetZoom write SetZoom;
    property printZoom: Integer read GetprintZoom write SetprintZoom;
    property landscape: boolean read Getlandscape write Setlandscape;
    property Paper: Paper read GetPaper write SetPaper;
    property header: AnsiString read Getheader write Setheader;
    property headerMargin: double read GetheaderMargin write SetheaderMargin;
    property footer: AnsiString read Getfooter write Setfooter;
    property footerMargin: double read GetfooterMargin write SetfooterMargin;
    property hCenter: boolean read GethCenter write SethCenter;
    property vCenter: boolean read GetvCenter write SetvCenter;

    property marginLeft: double read GetmarginLeft write SetmarginLeft;
    property marginRight: double read GetmarginRight write SetmarginRight;
    property marginTop: double read GetmarginTop write SetmarginTop;
    property marginBottom: double read GetmarginBottom write SetmarginBottom;

    property printRowCol: boolean read GetprintRowCol write SetprintRowCol;
    property name: AnsiString read GetName write SetName;
    property groupSummaryBelow: boolean read GetGroupSummaryBelow write SetGroupSummaryBelow;
    property groupSummaryRight: boolean read GetGroupSummaryRight write SetGroupSummaryRight;

    function ErrorMessage(): AnsiString;

    function getCellType(row: Integer; col: Integer): CellType;
    function cellFormat(row: Integer; col: Integer): TFormat;
    function isFormula(row: Integer; col: Integer): boolean;

    function readStr(row: Integer; col: Integer): AnsiString; overload;
    function readStr(row: Integer; col: Integer; var format: TFormat): AnsiString; overload;
    procedure writeStr(row: Integer; col: Integer; value: PAnsiChar); overload;
    procedure writeStr(row: Integer; col: Integer; value: PAnsiChar; format: TFormat); overload;

    function readNum(row: Integer; col: Integer): double; overload;
    function readNum(row: Integer; col: Integer; var format: TFormat): double; overload;
    procedure writeNum(row: Integer; col: Integer; value: double); overload;
    procedure writeNum(row: Integer; col: Integer; value: double; format: TFormat); overload;

    function readBool(row: Integer; col: Integer): boolean; overload;
    function readBool(row: Integer; col: Integer; var format: TFormat): boolean; overload;
    procedure writeBool(row: Integer; col: Integer; value: boolean); overload;
    procedure writeBool(row: Integer; col: Integer; value: boolean; format: TFormat); overload;

    procedure readBlank(row: Integer; col: Integer; var format: TFormat);
    procedure writeBlank(row: Integer; col: Integer; format: TFormat);

    function readFormula(row: Integer; col: Integer): AnsiString; overload;
    function readFormula(row: Integer; col: Integer; var format: TFormat): AnsiString; overload;
    procedure writeFormula(row: Integer; col: Integer; value: PAnsiChar); overload;
    procedure writeFormula(row: Integer; col: Integer; value: PAnsiChar; format: TFormat); overload;

    function readComment(row: Integer; col: Integer): AnsiString;
    procedure writeComment(row: Integer; col: Integer; value: PAnsiChar; author: PAnsiChar; width, height: Integer);

    function isDate(row, col: Integer): boolean;

    function readError(row, col: Integer): ErrorType;

    function colWidth(col: Integer): double;
    function rowHeight(col: Integer): double;

    procedure setCol(col: Integer; width: double); overload;
    procedure setCol(colFirst, colLast: Integer; width: double); overload;
    procedure setCol(col: Integer; width: double; format: TFormat); overload;
    procedure setCol(colFirst, colLast: Integer; width: double; format: TFormat); overload;
    procedure setCol(col: Integer; width: double; format: TFormat; hidden: boolean); overload;
    procedure setCol(colFirst, colLast: Integer; width: double; format: TFormat; hidden: boolean); overload;

    procedure setRow(row: Integer; height: double); overload;
    procedure setRow(row: Integer; height: double; format: TFormat); overload;
    procedure setRow(row: Integer; height: double; format: TFormat; hidden: boolean); overload;

    procedure getMerge(row, col: Integer; var rowFirst, rowLast, colFirst, colLast: Integer);
    procedure setMerge(rowFirst, rowLast, colFirst, colLast: Integer);
    procedure delMerge(row, col: Integer);

    procedure setPicture(row, col, pictureId: Integer); overload;
    procedure setPicture(row, col, pictureId: Integer; scale: double); overload;
    procedure setPicture(row, col, pictureId, width, height: Integer); overload;

    procedure setHorPageBreak(row: Integer);
    procedure delHorPageBreak(row: Integer);
    procedure setVerPageBreak(col: Integer);
    procedure delVerPageBreak(col: Integer);

    procedure split(row, col: Integer);

    procedure groupRows(rowFirst, rowLast: Integer; collapsed: boolean);
    procedure groupCols(colFirst, colLast: Integer; collapsed: boolean);

    procedure clear(rowFirst, rowLast: Integer; colFirst, colLast: Integer);

    procedure insertCol(colFirst, colLast: Integer);
    procedure insertRow(rowFirst, rowLast: Integer);
    procedure removeCol(colFirst, colLast: Integer);
    procedure removeRow(rowFirst, rowLast: Integer);

    procedure copyCell(rowSrc, colSrc, rowDst, colDst: Integer);

    function firstRow: Integer;
    function lastRow: Integer;
    function firstCol: Integer;
    function lastCol: Integer;

    procedure setPrintRepeatRows(rowFirst, rowLast: Integer);
    procedure setPrintRepeatCols(colFirst, colLast: Integer);
    procedure setPrintArea(rowFirst, rowLast, colFirst, colLast: Integer);
    
    procedure clearPrintRepeats();
    procedure clearPrintArea();

    procedure setNamedRange(const name: PAnsiChar; rowFirst, rowLast, colFirst, colLast: Integer);
    procedure delNamedRange(const name: PAnsiChar);

  published
    constructor Create(handle: SheetHandle; book: BookHandle);
    destructor Destroy; override;
  end;

type
  // {$M+}
  TBook = class(TObject)
  private
    handle: BookHandle;
    procedure Release;
  protected
  public
    procedure load(filename: PAnsiChar);
    procedure save(filename: PAnsiChar);

    procedure loadRaw(data: PByteArray; size: Cardinal);
    procedure saveRaw(var Buffer: ByteArray; var size: Cardinal);

    function addSheet(name: PAnsiChar): TSheet; overload;
    function addSheet(name: PAnsiChar; initSheet: TSheet): TSheet; overload;
    function getSheet(index: Integer): TSheet;
    procedure delSheet(index: Integer);
    function sheetCount(): Integer;

    function addFormat(): TFormat; overload;
    function addFormat(initFormat: TFormat): TFormat; overload;
    function addFont(): TFont; overload;
    function addFont(initFont: TFont): TFont; overload;
    function addCustomNumFormat(customNumFormat: PAnsiChar): Integer;
    function customNumFormat(fmt: Integer): AnsiString;

    function format(index: Integer): TFormat;
    function formatSize(): Integer;

    function font(index: Integer): TFont;
    function fontSize(): Integer;

    function datePack(year: Integer; month: Integer; day: Integer): double; overload;
    function datePack(year: Integer; month: Integer; day: Integer; hour: Integer; min: Integer; sec: Integer): double; overload;
    function datePack(year: Integer; month: Integer; day: Integer; hour: Integer; min: Integer; sec: Integer; msec: Integer): double; overload;

    procedure dateUnpack(value: double; var year: Integer; var month: Integer; var day: Integer); overload;
    procedure dateUnpack(value: double; var year: Integer; var month: Integer; var day: Integer; var hour: Integer; var min: Integer; var sec: Integer); overload;
    procedure dateUnpack(value: double; var year: Integer; var month: Integer; var day: Integer; var hour: Integer; var min: Integer; var sec: Integer; var msec: Integer); overload;

    function colorPack(red: Integer; green: Integer; blue: Integer): Color;
    procedure colorUnpack(color: Color; var red: Integer; var green: Integer; var blue);

    function activeSheet(): Integer;
    procedure setActiveSheet(index: Integer);

    function addPicture(filename: PAnsiChar): Integer;

    function defaultFont(var fontSize: Integer): AnsiString;
    procedure setDefaultFont(fontName: PAnsiChar; fontSize: Integer);

    procedure setKey(name: PWideChar; key: PWideChar);

    function rgbMode(): boolean;
    procedure setRgbMode(rgbMode: boolean);

    procedure setLocale(locale: PAnsiChar);

    function errorMessage(): AnsiString;

  published
    destructor Destroy; override;
  end;


  TBinBook = class(TBook)
  published
    constructor Create;    
  end;

  TXmlBook = class(TBook)
  published
    constructor Create;    
  end;
  

implementation


constructor ExceptionXL.Create(book: BookHandle);
begin
  inherited Create(AnsiString(xlBookErrorMessage(book)));
end;

function TFont.Error: AnsiString;
begin
  raise ExceptionXL.Create(book);
end;

constructor TFont.Create(handle: FontHandle; book: BookHandle);
begin
  inherited Create;
  self.book := book;
  self.handle := handle;
end;

destructor TFont.Destroy;
begin
  inherited Destroy;
end;

function TFont.Getbold: boolean;
begin
  result := (xlFontBold(handle) > 0);
end;

procedure TFont.Setbold(const value: boolean);
begin
  xlFontSetBold(handle, Integer(value));
end;

function TFont.GetName: AnsiString;
begin
  result := AnsiString(xlFontName(handle));
end;

procedure TFont.SetName(const value: AnsiString);
begin
  if xlFontSetName(handle, @(value[1])) = 0 then
    Error;
end;

function TFont.Getsize: Integer;
begin
  result := xlFontSize(handle);
end;

procedure TFont.Setsize(const value: Integer);
begin
  xlFontSetSize(handle, value);
end;

function TFont.GetUnderline: Underline;
begin
  result := LibXL.Underline(xlFontUnderline(handle));
end;

procedure TFont.SetUnderline(const value: LibXL.Underline);
begin
  xlFontSetUnderline(handle, Integer(value));
end;

function TFont.Getcolor: Color;
begin
  result := LibXL.Color(xlFontColor(handle));
end;

procedure TFont.Setcolor(const value: Color);
begin
  xlFontSetColor(handle, Integer(value));
end;

procedure TFont.Setitalic(const value: boolean);
begin
  xlFontSetItalic(handle, Integer(value));
end;

function TFont.Getitalic: boolean;
begin
  result := (xlFontItalic(handle) > 0);
end;

procedure TFont.Setscript(const value: Script);
begin
  xlFontSetScript(handle, Integer(value));
end;

function TFont.Getscript: Script;
begin
  result := LibXL.Script(xlFontScript(handle));
end;

procedure TFont.SetstrikeOut(const value: boolean);
begin
  xlFontSetStrikeOut(handle, Integer(value));
end;

function TFont.GetstrikeOut: boolean;
begin
  result := (xlFontStrikeOut(handle) > 0);
end;

constructor TFormat.Create(handle: FormatHandle; book: BookHandle);
begin
  inherited Create;
  self.book := book;
  self.handle := handle;
end;

destructor TFormat.Destroy;
begin
  inherited Destroy;
end;

procedure TFormat.Error;
begin
  raise ExceptionXL.Create(book);
end;

procedure TFormat.SetborderDiagonal(const value: BorderDiagonal);
begin
  xlFormatSetBorderDiagonal(handle, Integer(value));
end;

procedure TFormat.SetborderDiagonalColor(const value: Color);
begin
  xlFormatSetBorderDiagonalColor(handle, Integer(value));
end;

procedure TFormat.SetborderLeft(const value: BorderStyle);
begin
  xlFormatSetBorderLeft(handle, Integer(value));
end;

procedure TFormat.SetborderLeftColor(const value: Color);
begin
  xlFormatSetBorderLeftColor(handle, Integer(value));
end;

procedure TFormat.SetborderRight(const value: BorderStyle);
begin
  xlFormatSetBorderRight(handle, Integer(value));
end;

procedure TFormat.SetborderRightColor(const value: Color);
begin
  xlFormatSetBorderRightColor(handle, Integer(value));
end;

procedure TFormat.SetborderTop(const value: BorderStyle);
begin
  xlFormatSetBorderTop(handle, Integer(value));
end;

procedure TFormat.SetborderTopColor(const value: Color);
begin
  xlFormatSetBorderTopColor(handle, Integer(value));
end;

procedure TFormat.SetfillPattern(const value: FillPattern);
begin
  xlFormatSetFillPattern(handle, Integer(value));
end;

procedure TFormat.Setfont(const value: TFont);
begin
  if xlFormatSetFont(handle, value.handle) = nil then
    Error;
end;

function TFormat.GetalignH: AlignH;
begin
  result := LibXL.AlignH(xlFormatAlignH(handle));
end;

function TFormat.GetAlignV: AlignV;
begin
  result := LibXL.AlignV(xlFormatAlignV(handle));
end;

function TFormat.GetborderBottom: BorderStyle;
begin
  result := BorderStyle(xlFormatBorderBottom(handle));
end;

function TFormat.GetborderBottomColor: Color;
begin
  result := Color(xlFormatBorderBottomColor(handle));
end;

function TFormat.GetborderDiagonal: BorderDiagonal;
begin
  result := LibXL.BorderDiagonal(xlFormatBorderDiagonal(handle));
end;

function TFormat.GetborderDiagonalColor: Color;
begin
  result := Color(xlFormatBorderDiagonalColor(handle));
end;

function TFormat.GetborderLeft: BorderStyle;
begin
  result := BorderStyle(xlFormatBorderLeft(handle));
end;

function TFormat.GetborderLeftColor: Color;
begin
  result := Color(xlFormatBorderLeftColor(handle));
end;

function TFormat.GetborderRight: BorderStyle;
begin
  result := BorderStyle(xlFormatBorderRight(handle));
end;

function TFormat.GetborderRightColor: Color;
begin
  result := Color(xlFormatBorderRightColor(handle));
end;

function TFormat.GetborderTop: BorderStyle;
begin
  result := BorderStyle(xlFormatBorderTop(handle));
end;

function TFormat.GetborderTopColor: Color;
begin
  result := Color(xlFormatBorderTopColor(handle));
end;

function TFormat.GetfillPattern: FillPattern;
begin
  result := LibXL.FillPattern(xlFormatFillPattern(handle));
end;

procedure TFormat.Sethidden(const value: boolean);
begin
  xlFormatSetHidden(handle, Integer(value));
end;

procedure TFormat.Setindent(const value: Integer);
begin
  xlFormatSetIndent(handle, Integer(value));
end;

procedure TFormat.Setlocked(const value: boolean);
begin
  xlFormatSetLocked(handle, Integer(value));
end;

function TFormat.Getfont: TFont;
var
  f: FontHandle;
begin
  f := xlFormatFont(handle);
  if f = nil then
    Error;
  result := TFont.Create(f, book);
end;

function TFormat.Gethidden: boolean;
begin
  result := (xlFormatHidden(handle) > 0);
end;

function TFormat.Getindent: Integer;
begin
  result := (xlFormatIndent(handle));
end;

function TFormat.Getlocked: boolean;
begin
  result := (xlFormatLocked(handle) > 0);
end;

function TFormat.GetpatternBackgroundColor: Color;
begin
  result := Color(xlFormatPatternBackgroundColor(handle));
end;

function TFormat.GetpatternForegroundColor: Color;
begin
  result := Color(xlFormatPatternForegroundColor(handle));
end;

function TFormat.Getrotation: Integer;
begin
  result := xlFormatRotation(handle);
end;

function TFormat.GetshrinkToFit: boolean;
begin
  result := (xlFormatShrinkToFit(handle) > 0);
end;

function TFormat.Getwrap: boolean;
begin
  result := (xlFormatWrap(handle) > 0);
end;

function TFormat.NumFormat: Integer;
begin
  result := xlFormatNumFormat(handle);
end;

procedure TFormat.SetalignH(const value: AlignH);
begin
  xlFormatSetAlignH(handle, Integer(value));
end;

procedure TFormat.SetAlignV(const value: AlignV);
begin
  xlFormatSetAlignV(handle, Integer(value));
end;

procedure TFormat.setNumFormat(NumFormat: Integer);
begin
  xlFormatSetNumFormat(handle, NumFormat);
end;

procedure TFormat.setNumFormat(value: NumFormat);
begin
  xlFormatSetNumFormat(handle, Integer(value));
end;

procedure TFormat.SetpatternBackgroundColor(const value: Color);
begin
  xlFormatSetPatternBackgroundColor(handle, Integer(value));
end;

procedure TFormat.SetpatternForegroundColor(const value: Color);
begin
  xlFormatSetPatternForegroundColor(handle, Integer(value));
end;

procedure TFormat.Setrotation(const value: Integer);
begin
  if xlFormatSetRotation(handle, value) = 0 then
    Error;
end;

procedure TFormat.SetshrinkToFit(const value: boolean);
begin
  xlFormatSetShrinkToFit(handle, Integer(value));
end;

procedure TFormat.Setwrap(const value: boolean);
begin
  xlFormatSetWrap(handle, Integer(value));
end;

procedure TFormat.setBorder(style: BorderStyle);
begin
  xlFormatSetBorder(handle, Integer(style));
end;

procedure TFormat.SetborderBottom(const value: BorderStyle);
begin
  xlFormatSetBorderBottom(handle, Integer(value));
end;

procedure TFormat.SetborderBottomColor(const value: Color);
begin
  xlFormatSetBorderBottomColor(handle, Integer(value));
end;

procedure TFormat.setBorderColor(value: Color);
begin
  xlFormatSetBorderColor(handle, Integer(value));
end;


constructor TSheet.Create(handle: SheetHandle; book: BookHandle);
begin
  inherited Create;
  self.book := book;
  self.handle := handle;
end;

destructor TSheet.Destroy;
begin
  inherited Destroy;
end;

function TSheet.ErrorMessage: AnsiString;
begin
  result := AnsiString(xlBookErrorMessage(book));
end;

function TSheet.getCellType(row, col: Integer): CellType;
begin
  result := CellType(xlSheetCellType(handle, row, col));
end;

function TSheet.cellFormat(row, col: Integer): TFormat;
begin
  result := TFormat.Create(xlSheetCellFormat(handle, row, col), book);
end;

function TSheet.isFormula(row, col: Integer): boolean;
var
  res: Integer;
begin
  res := xlSheetIsFormula(handle, row, col);
  if res > 0 then
    result := true
  else
    result := false;
end;

function TSheet.readStr(row, col: Integer): AnsiString;
var
  format: FormatHandle;
  ret: AnsiString;
begin
  ret := AnsiString(xlSheetReadStr(handle, row, col, @format));
  if @ret = nil then
    Error;
  result := ret;
end;

function TSheet.readStr(row, col: Integer; var format: TFormat): AnsiString;
var
  fmtHandle: FormatHandle;
  ret: AnsiString;
begin
  ret := AnsiString(xlSheetReadStr(handle, row, col, @fmtHandle));
  if @ret = nil then
    Error;
  format := TFormat.Create(fmtHandle, book);
  result := ret;
end;

procedure TSheet.writeStr(row, col: Integer; value: PAnsiChar);
begin
  if xlSheetWriteStr(handle, row, col, value, nil) = 0 then
    Error;
end;

procedure TSheet.writeStr(row, col: Integer; value: PAnsiChar; format: TFormat);
begin
  if xlSheetWriteStr(handle, row, col, value, format.handle) = 0 then
    Error;
end;

function TSheet.readNum(row, col: Integer): double;
var
  format: FormatHandle;
begin
  result := xlSheetReadNum(handle, row, col, @format);
  if format = nil then
    Error;
end;

function TSheet.readNum(row, col: Integer; var format: TFormat): double;
begin
  result := xlSheetReadNum(handle, row, col, @format.handle);
  if format = nil then
    Error;
end;

procedure TSheet.writeNum(row, col: Integer; value: double);
begin
  if xlSheetWriteNum(handle, row, col, value, nil) = 0 then
    Error;
end;

procedure TSheet.writeNum(row, col: Integer; value: double; format: TFormat);
begin
  if xlSheetWriteNum(handle, row, col, value, format.handle) = 0 then
    Error;
end;

function TSheet.readBool(row, col: Integer): boolean;
var
  format: FormatHandle;
  res: Integer;
begin
  res := xlSheetReadBool(handle, row, col, @format);
  if format = nil then
    Error;
  if res > 0 then
    result := true
  else
    result := false;
end;

function TSheet.readBool(row, col: Integer; var format: TFormat): boolean;
var
  res: Integer;
begin
  res := xlSheetReadBool(handle, row, col, @format.handle);
  if format = nil then
    Error;
  if res > 0 then
    result := true
  else
    result := false;
end;

procedure TSheet.writeBool(row, col: Integer; value: boolean);
begin
  if xlSheetWriteBool(handle, row, col, Integer(value), nil) = 0 then
    Error;
end;

procedure TSheet.writeBool(row, col: Integer; value: boolean; format: TFormat);
begin
  if xlSheetWriteBool(handle, row, col, Integer(value), format.handle) = 0 then
    Error;
end;

procedure TSheet.readBlank(row, col: Integer; var format: TFormat);
begin
  if xlSheetReadBlank(handle, row, col, @format.handle) = 0 then
    Error;
end;

procedure TSheet.writeBlank(row, col: Integer; format: TFormat);
begin
  if xlSheetWriteBlank(handle, row, col, format.handle) = 0 then
    Error;
end;

function TSheet.readFormula(row, col: Integer): AnsiString;
var
  format: FormatHandle;
  ret: AnsiString;
begin
  ret := AnsiString(xlSheetReadFormula(handle, row, col, @format));
  if @ret = nil then
    Error;
  result := ret;
end;

function TSheet.readFormula(row, col: Integer; var format: TFormat): AnsiString;
var
  fmtHandle: FormatHandle;
  ret: AnsiString;
begin
  ret := AnsiString(xlSheetReadFormula(handle, row, col, @fmtHandle));
  if @ret = nil then
    Error;
  format := TFormat.Create(fmtHandle, book);
  result := ret;
end;

procedure TSheet.writeFormula(row, col: Integer; value: PAnsiChar);
begin
  if xlSheetWriteFormula(handle, row, col, value, nil) = 0 then
    Error;
end;

procedure TSheet.writeFormula(row, col: Integer; value: PAnsiChar; format: TFormat);
begin
  if xlSheetWriteFormula(handle, row, col, value, format.handle) = 0 then
    Error;
end;

function TSheet.readComment(row, col: Integer): AnsiString;
begin
  result := AnsiString(xlSheetReadComment(handle, row, col));
end;

procedure TSheet.writeComment(row, col: Integer; value, author: PAnsiChar; width, height: Integer);
begin
  xlSheetWriteComment(handle, row, col, value, author, width, height);
end;

function TSheet.isDate(row, col: Integer): boolean;
begin
  if xlSheetIsDate(handle, row, col) > 0 then
    result := true
  else
    result := false;
end;

function TSheet.readError(row, col: Integer): ErrorType;
begin
  result := ErrorType(xlSheetReadError(handle, row, col));
end;

function TSheet.colWidth(col: Integer): double;
begin
  result := xlSheetColWidth(handle, col);
end;

function TSheet.rowHeight(col: Integer): double;
begin
  result := xlSheetRowHeight(handle, col);
end;

procedure TSheet.setCol(col: Integer; width: double; format: TFormat);
begin
  setCol(col, col, width, format);
end;

procedure TSheet.setCol(colFirst, colLast: Integer; width: double);
begin
  if xlSheetSetCol(handle, colFirst, colLast, width, nil, 0) = 0 then
    Error;
end;

procedure TSheet.setCol(col: Integer; width: double);
begin
  setCol(col, col, width);
end;

procedure TSheet.setCol(colFirst, colLast: Integer; width: double; format: TFormat; hidden: boolean);
var
  fmtHandle: FormatHandle;
begin
  if format <> nil then
    fmtHandle := format.handle
  else
    fmtHandle := nil;
  if xlSheetSetCol(handle, colFirst, colLast, width, fmtHandle, Integer(hidden)) = 0 then
    Error;
end;

procedure TSheet.setCol(col: Integer; width: double; format: TFormat; hidden: boolean);
begin
  setCol(col, col, width, format, hidden);
end;

procedure TSheet.setCol(colFirst, colLast: Integer; width: double; format: TFormat);
begin
  if xlSheetSetCol(handle, colFirst, colLast, width, format.handle, 0) = 0 then
    Error;
end;

procedure TSheet.setRow(row: Integer; height: double);
begin
  if xlSheetSetRow(handle, row, height, nil, 0) = 0 then
    Error;
end;

procedure TSheet.setRow(row: Integer; height: double; format: TFormat);
begin
  if xlSheetSetRow(handle, row, height, format.handle, 0) = 0 then
    Error;
end;

procedure TSheet.setRow(row: Integer; height: double; format: TFormat; hidden: boolean);
var
  fmtHandle: FormatHandle;
begin
  if format <> nil then
    fmtHandle := format.handle
  else
    fmtHandle := nil;
  if xlSheetSetRow(handle, row, height, fmtHandle, Integer(hidden)) = 0 then
    Error;
end;

procedure TSheet.delMerge(row, col: Integer);
begin
  if xlSheetDelMerge(handle, row, col) = 0 then
    Error;
end;

procedure TSheet.getMerge(row, col: Integer; var rowFirst, rowLast, colFirst, colLast: Integer);
begin
  if xlSheetGetMerge(handle, row, col, @rowFirst, @rowLast, @colFirst, @colLast) = 0 then
    Error;
end;

procedure TSheet.setMerge(rowFirst, rowLast, colFirst, colLast: Integer);
begin
  if xlSheetSetMerge(handle, rowFirst, rowLast, colFirst, colLast) = 0 then
    Error;
end;

procedure TSheet.Error;
begin
  raise ExceptionXL.Create(book);
end;

procedure TSheet.delHorPageBreak(row: Integer);
begin
  if xlSheetSetHorPageBreak(handle, row, 0) = 0 then
    Error;
end;

procedure TSheet.delVerPageBreak(col: Integer);
begin
  if xlSheetSetVerPageBreak(handle, col, 0) = 0 then
    Error;
end;

procedure TSheet.setHorPageBreak(row: Integer);
begin
  if xlSheetSetHorPageBreak(handle, row, 1) = 0 then
    Error;
end;

procedure TSheet.setVerPageBreak(col: Integer);
begin
  if xlSheetSetVerPageBreak(handle, col, 1) = 0 then
    Error;
end;

procedure TSheet.setPicture(row, col, pictureId, width, height: Integer);
begin
  xlSheetSetPicture2(handle, row, col, pictureId, width, height);
  if ErrorMessage() <> 'ok' then
    Error;
end;

procedure TSheet.setPicture(row, col, pictureId: Integer; scale: double);
begin
  xlSheetSetPicture(handle, row, col, pictureId, scale);
  if ErrorMessage() <> 'ok' then
    Error;
end;

procedure TSheet.setPicture(row, col, pictureId: Integer);
begin
  setPicture(row, col, pictureId, 1);
end;

procedure TSheet.split(row, col: Integer);
begin
  xlSheetSplit(handle, row, col);
end;

procedure TSheet.groupCols(colFirst, colLast: Integer; collapsed: boolean);
begin
  if xlSheetGroupCols(handle, colFirst, colLast, Integer(collapsed)) = 0 then
    Error;
end;

procedure TSheet.groupRows(rowFirst, rowLast: Integer; collapsed: boolean);
begin
  if xlSheetGroupRows(handle, rowFirst, rowLast, Integer(collapsed)) = 0 then
    Error;
end;

procedure TSheet.clear(rowFirst, rowLast, colFirst, colLast: Integer);
begin
  xlSheetClear(handle, rowFirst, rowLast, colFirst, colLast);
end;

procedure TSheet.insertCol(colFirst, colLast: Integer);
begin
  xlSheetInsertCol(handle, colFirst, colLast);
end;

procedure TSheet.insertRow(rowFirst, rowLast: Integer);
begin
  xlSheetInsertRow(handle, rowFirst, rowLast);
end;

procedure TSheet.removeCol(colFirst, colLast: Integer);
begin
  xlSheetRemoveCol(handle, colFirst, colLast);
end;

procedure TSheet.removeRow(rowFirst, rowLast: Integer);
begin
  xlSheetRemoveRow(handle, rowFirst, rowLast);
end;

procedure TSheet.copyCell(rowSrc, colSrc, rowDst, colDst: Integer);
begin
  if xlSheetCopyCell(handle, rowSrc, colSrc, rowDst, colDst) = 0 then
    Error;
end;

procedure TSheet.setPrintRepeatRows(rowFirst, rowLast: Integer);
begin
  xlSheetSetPrintRepeatRows(handle, rowFirst, rowLast);
end;

procedure TSheet.setPrintRepeatCols(colFirst, colLast: Integer);
begin
  xlSheetSetPrintRepeatCols(handle, colFirst, colLast);
end;

procedure TSheet.setPrintArea(rowFirst, rowLast, colFirst, colLast: Integer);
begin
  xlSheetSetPrintArea(handle, rowFirst, rowLast, colFirst, colLast);
end;
    
procedure TSheet.clearPrintRepeats();
begin
  xlSheetClearPrintRepeats(handle);
end;

procedure TSheet.clearPrintArea();
begin
  xlSheetClearPrintArea(handle);
end;

procedure TSheet.setNamedRange(const name: PAnsiChar; rowFirst, rowLast, colFirst, colLast: Integer);
begin
  if xlSheetSetNamedRange(handle, name, rowFirst, rowLast, colFirst, colLast) = 0 then
    Error;
end;

procedure TSheet.delNamedRange(const name: PAnsiChar);
begin
  if xlSheetDelNamedRange(handle, name) = 0 then
    Error;
end;

function TSheet.firstRow: Integer;
begin
  result := xlSheetFirstRow(handle);
end;

function TSheet.lastRow: Integer;
begin
  result := xlSheetLastRow(handle);
end;

function TSheet.firstCol: Integer;
begin
  result := xlSheetFirstCol(handle);
end;

function TSheet.lastCol: Integer;
begin
  result := xlSheetLastCol(handle);
end;

function TSheet.GetDisplayGridlines: boolean;
begin
  result := (xlSheetDisplayGridlines(handle) > 0);
end;

procedure TSheet.SetDisplayGridlines(const value: boolean);
begin
  xlSheetSetDisplayGridlines(handle, Integer(value));
end;

function TSheet.GetprintGridlines: boolean;
begin
  result := (xlSheetPrintGridlines(handle) > 0);
end;

procedure TSheet.SetprintGridlines(const value: boolean);
begin
  xlSheetSetPrintGridlines(handle, Integer(value));
end;

procedure TSheet.SetZoom(const value: Integer);
begin
  xlSheetSetZoom(handle, value);
end;

function TSheet.GetZoom: Integer;
begin
  result := xlSheetZoom(handle);
end;

function TSheet.GetprintZoom: Integer;
begin
  result := xlSheetPrintZoom(handle);
end;

procedure TSheet.SetprintZoom(const value: Integer);
begin
  xlSheetSetPrintZoom(handle, value);
end;

procedure TSheet.Setlandscape(const value: boolean);
begin
  xlSheetSetLandscape(handle, Integer(value));
end;

function TSheet.Getlandscape: boolean;
begin
  result := (xlSheetLandscape(handle) > 0);
end;

function TSheet.GetPaper: Paper;
begin
  result := LibXL.Paper(xlSheetPaper(handle));
end;

procedure TSheet.SetPaper(const value: Paper);
begin
  xlSheetSetPaper(handle, Integer(value));
end;

function TSheet.Getheader: AnsiString;
var
  s: PAnsiChar;
begin
  s := xlSheetHeader(handle);
  if s = nil then
    Error;
  result := AnsiString(s);
end;

procedure TSheet.Setheader(const value: AnsiString);
var
  PValue: PAnsiChar;
begin
  PValue := @(value[1]);
  if xlSheetSetHeader(handle, PValue, 0.5) = 0 then
    Error;
end;

function TSheet.GetheaderMargin: double;
begin
  result := xlSheetHeaderMargin(handle);
end;

procedure TSheet.SetheaderMargin(const value: double);
begin
  if xlSheetSetHeader(handle, @(self.header[1]), value) = 0 then
    Error;
end;

function TSheet.Getfooter: AnsiString;
var
  s: PAnsiChar;
begin
  s := xlSheetFooter(handle);
  if s = nil then
    Error;
  result := AnsiString(s);
end;

procedure TSheet.Setfooter(const value: AnsiString);
var
  PValue: PAnsiChar;
begin
  PValue := @(value[1]);
  if xlSheetSetFooter(handle, PValue, 0.5) = 0 then
    Error;
end;

function TSheet.GetfooterMargin: double;
begin
  result := xlSheetHeaderMargin(handle);
end;

procedure TSheet.SetfooterMargin(const value: double);
begin
  if xlSheetSetHeader(handle, @(self.footer[1]), value) = 0 then
    Error;
end;

procedure TSheet.SethCenter(const value: boolean);
begin
  xlSheetSetHCenter(handle, Integer(value));
end;

function TSheet.GethCenter: boolean;
begin
  result := (xlSheetHCenter(handle) > 0);
end;

procedure TSheet.SetvCenter(const value: boolean);
begin
  xlSheetSetVCenter(handle, Integer(value));
end;

function TSheet.GetvCenter: boolean;
begin
  result := (xlSheetVCenter(handle) > 0);
end;

procedure TSheet.SetmarginLeft(const value: double);
begin
  xlSheetSetMarginLeft(handle, value);
end;

function TSheet.GetmarginLeft: double;
begin
  result := xlSheetMarginLeft(handle);
end;

procedure TSheet.SetmarginRight(const value: double);
begin
  xlSheetSetMarginRight(handle, value);
end;

function TSheet.GetmarginRight: double;
begin
  result := xlSheetMarginRight(handle);
end;

procedure TSheet.SetmarginTop(const value: double);
begin
  xlSheetSetMarginTop(handle, value);
end;

function TSheet.GetmarginTop: double;
begin
  result := xlSheetMarginTop(handle);
end;

procedure TSheet.SetmarginBottom(const value: double);
begin
  xlSheetSetMarginBottom(handle, value);
end;

function TSheet.GetmarginBottom: double;
begin
  result := xlSheetMarginBottom(handle);
end;

function TSheet.Getname: AnsiString;
begin
  result := AnsiString(xlSheetName(handle));
end;

procedure TSheet.Setname(const value: AnsiString);
begin
  xlSheetSetName(handle, @(value[1]));
end;

function TSheet.GetprintRowCol: boolean;
begin
  result := (xlSheetPrintRowCol(handle) > 0);
end;

procedure TSheet.SetprintRowCol(const value: boolean);
begin
  xlSheetSetPrintRowCol(handle, Integer(value));
end;

function TSheet.GetGroupSummaryBelow: boolean;
begin
  result := (xlSheetGroupSummaryBelow(handle) > 0);
end;

procedure TSheet.SetGroupSummaryBelow(const value: boolean);
begin
  xlSheetSetGroupSummaryBelow(handle, Integer(value));
end;

function TSheet.GetGroupSummaryRight: boolean;
begin
  result := (xlSheetGroupSummaryRight(handle) > 0);
end;

procedure TSheet.SetGroupSummaryRight(const value: boolean);
begin
  xlSheetSetGroupSummaryRight(handle, Integer(value));
end;

procedure TBook.load(filename: PAnsiChar);
begin
  if xlBookLoad(handle, filename) = 0 then
    raise ExceptionXL.Create(handle);
end;

procedure TBook.save(filename: PAnsiChar);
begin
  if xlBookSave(handle, filename) = 0 then
    raise ExceptionXL.Create(handle);
end;

procedure TBook.loadRaw(data: PByteArray; size: Cardinal);
begin
  if xlBookLoadRaw(handle, addr(data^[0]), size) > 0 then
    raise ExceptionXL.Create(handle);
end;

procedure TBook.saveRaw(var Buffer: ByteArray; var size: Cardinal);
var
  ptr: PByteArray;
  i: Integer;
  p: PByte;
begin
  if xlBookSaveRaw(handle, @ptr, @size) > 0 then
    raise ExceptionXL.Create(handle)
  else
  begin
    SetLength(Buffer, size);
    p := @(ptr^);
    for i := 0 to size - 1 do
    begin
      Buffer[i] := Byte(p^);
      Inc(p);
    end;
  end;
end;

function TBook.addSheet(name: PAnsiChar): TSheet;
var
  new_sheetHandle: SheetHandle;
begin
  new_sheetHandle := xlBookAddSheet(handle, name, nil);
  if new_sheetHandle = nil then
    raise ExceptionXL.Create(handle);
  result := TSheet.Create(new_sheetHandle, self.handle);
end;

function TBook.addSheet(name: PAnsiChar; initSheet: TSheet): TSheet;
var
  new_sheetHandle: SheetHandle;
begin
  new_sheetHandle := xlBookAddSheet(handle, name, initSheet.handle);
  if new_sheetHandle = nil then
    raise ExceptionXL.Create(handle);
  result := TSheet.Create(new_sheetHandle, self.handle);
end;

function TBook.getSheet(index: Integer): TSheet;
var
  new_sheetHandle: SheetHandle;
begin
  new_sheetHandle := xlBookGetSheet(handle, index);
  if new_sheetHandle = nil then
    raise ExceptionXL.Create(handle);
  result := TSheet.Create(new_sheetHandle, self.handle);
end;

procedure TBook.delSheet(index: Integer);
begin
  if xlBookDelSheet(handle, index) = 0 then
    raise ExceptionXL.Create(handle);
end;

function TBook.sheetCount: Integer;
begin
  result := xlBookSheetCount(handle);
end;

function TBook.addFormat: TFormat;
var
  fmtHandle: FormatHandle;
begin
  fmtHandle := xlBookAddFormat(handle, nil);
  if fmtHandle = nil then
    raise ExceptionXL.Create(handle);
  result := TFormat.Create(fmtHandle, handle);
end;

function TBook.addFormat(initFormat: TFormat): TFormat;
var
  fmtHandle: FormatHandle;
begin
  fmtHandle := xlBookAddFormat(handle, initFormat.handle);
  if fmtHandle = nil then
    raise ExceptionXL.Create(handle);
  result := TFormat.Create(fmtHandle, handle);
end;

function TBook.addFont: TFont;
var
  font: FontHandle;
begin
  font := xlBookAddFont(handle, nil); ;
  if font = nil then
    raise ExceptionXL.Create(handle);
  result := TFont.Create(font, handle);
end;

function TBook.addFont(initFont: TFont): TFont;
var
  font: FontHandle;
begin
  font := xlBookAddFont(handle, initFont.handle); ;
  if font = nil then
    raise ExceptionXL.Create(handle);
  result := TFont.Create(font, handle);
end;

function TBook.addCustomNumFormat(customNumFormat: PAnsiChar): Integer;
var
  fmt: Integer;
begin
  fmt := xlBookAddCustomNumFormat(handle, customNumFormat);
  if fmt = 0 then
    raise ExceptionXL.Create(handle);
  result := fmt;
end;

function TBook.customNumFormat(fmt: Integer): AnsiString;
var
  s: AnsiString;
begin
  s := AnsiString(xlBookCustomNumFormat(handle, fmt));
  if @s = nil then
    raise ExceptionXL.Create(handle);
  result := s;
end;

function TBook.format(index: Integer): TFormat;
var
  fmtHandle : FormatHandle;
begin
  fmtHandle := xlBookFormat(handle, index);
  if fmtHandle = nil then
    raise ExceptionXL.Create(handle);
  result := TFormat.Create(fmtHandle, handle);
end;

function TBook.formatSize(): Integer;
begin
  result := xlBookFormatSize(handle);
end;

function TBook.font(index: Integer): TFont;
var
  fntHandle : FontHandle;
begin
  fntHandle := xlBookFont(handle, index);
  if fntHandle = nil then
    raise ExceptionXL.Create(handle);
  result := TFont.Create(fntHandle, handle);
end;

function TBook.fontSize(): Integer;
begin
  result := xlBookFontSize(handle);
end;

function TBook.datePack(year, month, day: Integer): double;
begin
  result := xlBookDatePack(handle, year, month, day, 0, 0, 0, 0);
end;

function TBook.datePack(year, month, day, hour, min, sec: Integer): double;
begin
  result := xlBookDatePack(handle, year, month, day, hour, min, sec, 0);
end;

function TBook.datePack(year, month, day, hour, min, sec, msec: Integer): double;
begin
  result := xlBookDatePack(handle, year, month, day, hour, min, sec, msec);
end;

procedure TBook.dateUnpack(value: double; var year: Integer; var month: Integer; var day: Integer);
begin
  if xlBookDateUnpack(handle, value, addr(year), addr(month), addr(day), nil, nil, nil, nil) = 0 then
    raise ExceptionXL.Create(handle);
end;

procedure TBook.dateUnpack(value: double; var year: Integer; var month: Integer; var day: Integer; var hour: Integer; var min: Integer; var sec: Integer);
var
  result: Integer;
begin
  result := xlBookDateUnpack(handle, value, addr(year), addr(month), addr(day), addr(hour), addr(min), addr(sec), nil);
  if result = 0 then
    raise ExceptionXL.Create(handle);
end;

procedure TBook.dateUnpack(value: double; var year: Integer; var month: Integer; var day: Integer; var hour: Integer; var min: Integer; var sec: Integer; var msec: Integer);
var
  result: Integer;
begin
  result := xlBookDateUnpack(handle, value, addr(year), addr(month), addr(day), addr(hour), addr(min), addr(sec), addr(msec));
  if result = 0 then
    raise ExceptionXL.Create(handle);
end;

function TBook.colorPack(red: Integer; green: Integer; blue: Integer): Color;
begin
  result := Color(xlBookColorPack(handle, red, green, blue));
end;

procedure TBook.colorUnpack(color: Color; var red: Integer; var green: Integer; var blue);
begin
  xlBookColorUnpack(handle, Integer(color), addr(red), addr(green), addr(blue));
end;

function TBook.activeSheet: Integer;
begin
  result := xlBookActiveSheet(handle);
end;

procedure TBook.setActiveSheet(index: Integer);
begin
  xlBookSetActiveSheet(handle, index);
end;

function TBook.addPicture(filename: PAnsiChar): Integer;
begin
  result := xlBookAddPicture(handle, filename);
  if result = -1 then
    raise ExceptionXL.Create(handle);
end;

function TBook.defaultFont(var fontSize: Integer): AnsiString;
var
  s: PAnsiChar;
begin
  s := xlBookDefaultFont(handle, addr(fontSize));
  if s = nil then
    raise ExceptionXL.Create(handle);
  result := AnsiString(s);
end;

procedure TBook.setDefaultFont(fontName: PAnsiChar; fontSize: Integer);
begin
  xlBookSetDefaultFont(handle, fontName, fontSize);
end;

procedure TBook.setKey(name, key: PWideChar);
begin
  xlBookSetKey(handle, name, key);
end;

function TBook.rgbMode(): boolean;
begin
  result := boolean(xlBookRgbMode(handle));
end;

procedure TBook.setRgbMode(rgbMode: boolean);
begin
  xlBookSetRgbMode(handle, Integer(rgbMode));
end;

procedure TBook.setLocale(locale: PAnsiChar);
begin
  xlBookSetLocale(handle, locale);
end;


function TBook.errorMessage: AnsiString;
begin
  result := AnsiString(xlBookErrorMessage(handle));
end;


procedure TBook.Release;
begin
  try
    xlBookRelease(handle);
  finally
  end;
end;

destructor TBook.Destroy;
begin
  Release;
  inherited Destroy;
end;

constructor TBinBook.Create();
begin
  inherited Create;
  handle := xlCreateBookC;
end;

constructor TXmlBook.Create();
begin
  inherited Create;
  handle := xlCreateXMLBookC;
end;


end.
