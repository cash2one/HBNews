using System;
using System.Runtime.InteropServices;
using System.Text;

namespace libxl
{
    public class Sheet
    {
        public IntPtr handle;
        Book book;

        public Sheet(IntPtr handle, Book book)
        {
            this.handle = handle;
            this.book = book;
        }

        public CellType cellType(int row, int col)
        {
            return (CellType)xlSheetCellType(handle, row, col);            
        }

        public Format cellFormat(int row, int col)
        {
            IntPtr formatHandle = xlSheetCellFormat(handle, row, col);
            return new Format(formatHandle, book);
        }

        public void setCellFormat(int row, int col, Format format)
        {
            xlSheetSetCellFormat(handle, row, col, format.handle);
        }

        public bool isFormula(int row, int col)
        {
            return xlSheetIsFormula(handle, row, col) > 0 ? true : false;
        }

        public string readStr(int row, int col)
        {
            IntPtr formatHandle = (IntPtr)0;
            string s = xlSheetReadStr(handle, row, col, ref formatHandle);
            if (s == null)
            {
                throw new Exception(book.errorMessage());
            }
            return s;
        }

        public string readStr(int row, int col, ref Format format)
        {
            IntPtr formatHandle = (IntPtr)0;
            string s = xlSheetReadStr(handle, row, col, ref formatHandle);
            if (s == null)
            {
                throw new Exception(book.errorMessage());
            }
            format = new Format(formatHandle, book);
            return s;
        }

        public void writeStr(int row, int col, string value)
        {
            int result = xlSheetWriteStr(handle, row, col, value, (IntPtr)0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void writeStr(int row, int col, string value, Format format)
        {
            int result = xlSheetWriteStr(handle, row, col, value, format.handle);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public double readNum(int row, int col)
        {
            IntPtr formatHandle = (IntPtr)0;
            double result = xlSheetReadNum(handle, row, col, ref formatHandle);
            if (formatHandle == null)
            {
                throw new Exception(book.errorMessage());
            }
            return result;
        }

        public double readNum(int row, int col, ref Format format)
        {
            double result = xlSheetReadNum(handle, row, col, ref format.handle);
            if (format.handle == null)
            {
                throw new Exception(book.errorMessage());
            }
            return result;
        }

        public void writeNum(int row, int col, double value)
        {
            int result = xlSheetWriteNum(handle, row, col, value, (IntPtr)0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void writeNum(int row, int col, double value, Format format)
        {
            int result = xlSheetWriteNum(handle, row, col, value, format.handle);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public bool readBool(int row, int col)
        {
            IntPtr formatHandle = (IntPtr)0;
            int result = xlSheetReadBool(handle, row, col, ref formatHandle);
            if (formatHandle == null)
            {
                throw new Exception(book.errorMessage());
            }
            return result > 0 ? true : false;
        }

        public bool readBool(int row, int col, ref Format format)
        {
            int result = xlSheetReadBool(handle, row, col, ref format.handle);
            if (format.handle == null)
            {
                throw new Exception(book.errorMessage());
            }
            return result > 0 ? true : false;
        }

        public void writeBool(int row, int col, bool value)
        {
            int result = xlSheetWriteBool(handle, row, col, value ? 1 : 0, (IntPtr)0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void writeBool(int row, int col, bool value, Format format)
        {
            int result = xlSheetWriteBool(handle, row, col, value ? 1 : 0, format.handle);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void readBlank(int row, int col, ref Format format)
        {
            int result = xlSheetReadBlank(handle, row, col, ref format.handle);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void writeBlank(int row, int col, Format format)
        {
            int result = xlSheetWriteBlank(handle, row, col, format.handle);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public string readFormula(int row, int col)
        {
            IntPtr formatHandle = (IntPtr)0;
            string s = xlSheetReadFormula(handle, row, col, ref formatHandle);
            if (s == null)
            {
                throw new Exception(book.errorMessage());
            }
            return s;
        }

        public string readFormula(int row, int col, ref Format format)
        {
            IntPtr formatHandle = (IntPtr)0;
            string s = xlSheetReadFormula(handle, row, col, ref formatHandle);
            if (s == null)
            {
                throw new Exception(book.errorMessage());
            }
            format = new Format(formatHandle, book);
            return s;
        }

        public void writeFormula(int row, int col, string value)
        {
            int result = xlSheetWriteFormula(handle, row, col, value, (IntPtr)0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void writeFormula(int row, int col, string value, Format format)
        {
            int result = xlSheetWriteFormula(handle, row, col, value, format.handle);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public string readComment(int row, int col)
        {
            return xlSheetReadComment(handle, row, col);
        }

        public void writeComment(int row, int col, string value, string author, int width, int height)
        {
            xlSheetWriteComment(handle, row, col, value, author, width, height);            
        }

        public bool isDate(int row, int col)
        {            
            return  xlSheetIsDate(handle, row, col) > 0 ? true : false;
        }

        public ErrorType readError(int row, int col)
        {
            return (ErrorType)xlSheetReadError(handle, row, col);
        }

        public double colWidth(int col)
        {
            return xlSheetColWidth(handle, col);
        }

        public double rowHeight(int row)
        {
            return xlSheetRowHeight(handle, row);
        }

        public void setCol(int col, double width)
        {
            setCol(col, col, width);
        }

        public void setCol(int colFirst, int colLast, double width)
        {
            int result = xlSheetSetCol(handle, colFirst, colLast, width, (IntPtr)0, 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void setCol(int col, double width, Format format)
        {
            setCol(col, col, width, format);
        }

        public void setCol(int colFirst, int colLast, double width, Format format)
        {
            int result = xlSheetSetCol(handle, colFirst, colLast, width, format.handle, 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void setCol(int col, double width, Format format, bool hidden)
        {
            setCol(col, col, width, format, hidden);
        }

        public void setCol(int colFirst, int colLast, double width, Format format, bool hidden)
        {           
            
            int result = xlSheetSetCol(handle, colFirst, colLast, width, (format == null) ? (IntPtr)0 : format.handle, hidden ? 1 : 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }            
        }

        public void setRow(int row, double height)
        {
            int result = xlSheetSetRow(handle, row, height, (IntPtr)0, 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void setRow(int row, double height, Format format)
        {
            int result = xlSheetSetRow(handle, row, height, format.handle, 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void setRow(int row, double height, Format format, bool hidden)
        {
            int result = xlSheetSetRow(handle, row, height, (format == null) ? (IntPtr)0 : format.handle, hidden ? 1 : 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void getMerge(int row, int col, ref int rowFirst, ref int rowLast, ref int colFirst, ref int colLast)
        {
            int result = xlSheetGetMerge(handle, row, col, ref rowFirst, ref rowLast, ref colFirst, ref colLast);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void setMerge(int rowFirst, int rowLast, int colFirst, int colLast)
        {
            int result = xlSheetSetMerge(handle, rowFirst, rowLast, colFirst, colLast);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void delMerge(int row, int col)
        {
            int result = xlSheetDelMerge(handle, row, col);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void setPicture(int row, int col, int pictureId)
        {
            setPicture(row, col, pictureId, 1);
        }

        public void setPicture(int row, int col, int pictureId, double scale)
        {
            xlSheetSetPicture(handle, row, col, pictureId, scale);
            if (book.errorMessage() != "ok")
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void setPicture(int row, int col, int pictureId, int width, int height)
        {
            xlSheetSetPicture2(handle, row, col, pictureId, width, height);
            if (book.errorMessage() != "ok")
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void setHorPageBreak(int row)
        {
            int result = xlSheetSetHorPageBreak(handle, row, 1);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void delHorPageBreak(int row)
        {
            int result = xlSheetSetHorPageBreak(handle, row, 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void setVerPageBreak(int col)
        {
            int result = xlSheetSetVerPageBreak(handle, col, 1);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void delVerPageBreak(int col)
        {
            int result = xlSheetSetVerPageBreak(handle, col, 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void split(int row, int col)
        {
            xlSheetSplit(handle, row, col);
        }

        public void split2(int x, int y, int rowTop, int colLeft, bool frozen, PaneType paneType)
        {
            xlSheetSplit2(handle, x, y, rowTop, colLeft, frozen ? 1 : 0, (int)paneType);
        }

        public void groupRows(int rowFirst, int rowLast, bool collapsed)
        {
            int result = xlSheetGroupRows(handle, rowFirst, rowLast, collapsed ? 1 : 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void groupCols(int colFirst, int colLast, bool collapsed)
        {
            int result = xlSheetGroupCols(handle, colFirst, colLast, collapsed ? 1 : 0);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public bool groupSummaryBelow
        {
            get { return xlSheetGroupSummaryBelow(handle) > 0; }
            set { xlSheetSetGroupSummaryBelow(handle, value ? 1 : 0); }
        }

        public bool groupSummaryRight
        {
            get { return xlSheetGroupSummaryRight(handle) > 0; }
            set { xlSheetSetGroupSummaryRight(handle, value ? 1 : 0); }
        }

        public void clear(int rowFirst, int rowLast, int colFirst, int colLast)
        {
            xlSheetClear(handle, rowFirst, rowLast, colFirst, colLast);
        }

        public void insertCol(int colFirst, int colLast)
        {
            xlSheetInsertCol(handle, colFirst, colLast);
        }

        public void insertRow(int rowFirst, int rowLast)
        {
            xlSheetInsertRow(handle, rowFirst, rowLast);
        }

        public void removeCol(int colFirst, int colLast)
        {
            xlSheetRemoveCol(handle, colFirst, colLast);
        }

        public void removeRow(int rowFirst, int rowLast)
        {
            xlSheetRemoveRow(handle, rowFirst, rowLast);
        }

        public void copyCell(int rowSrc, int colSrc, int rowDst, int colDst)
        {
            int result = xlSheetCopyCell(handle, rowSrc, colSrc, rowDst, colDst);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public int firstRow()
        {
            return xlSheetFirstRow(handle);
        }

        public int lastRow()
        {
            return xlSheetLastRow(handle);
        }

        public int firstCol()
        {
            return xlSheetFirstCol(handle);
        }

        public int lastCol()
        {
            return xlSheetLastCol(handle);
        }

        public bool displayGridlines
        {
            get { return xlSheetDisplayGridlines(handle) > 0; }
            set { xlSheetSetDisplayGridlines(handle, value ? 1 : 0); }
        }

        public bool printGridlines
        {
            get { return xlSheetPrintGridlines(handle) > 0; }
            set { xlSheetSetPrintGridlines(handle, value ? 1 : 0); }
        }

        public int zoom
        {
            get { return xlSheetZoom(handle); }
            set { xlSheetSetZoom(handle, value); }
        }

        public int printZoom
        {
            get { return xlSheetPrintZoom(handle); }
            set { xlSheetSetPrintZoom(handle, value); }
        }

        public bool landscape
        {
            get { return xlSheetLandscape(handle) > 0; }
            set { xlSheetSetLandscape(handle, value ? 1 : 0); }
        }

        public Paper paper
        {
            get { return (Paper)xlSheetPaper(handle); }
            set { xlSheetSetPaper(handle, (int)value); }
        }

        public string header
        {
            get
            {
                string s = xlSheetHeader(handle);
                if (s == null)
                {
                    throw new Exception(book.errorMessage());
                }
                return s;
            }

            set
            {
                int result = xlSheetSetHeader(handle, value, 0.5);
                if (result == 0)
                {
                    throw new Exception(book.errorMessage());
                }
            }
        }

        public double headerMargin
        {
            get
            {
                return xlSheetHeaderMargin(handle);
            }

            set
            {
                int result = xlSheetSetHeader(handle, header, value);
                if (result == 0)
                {
                    throw new Exception(book.errorMessage());
                }
            }
        }

        public string footer
        {
            get
            {
                string s = xlSheetFooter(handle);
                if (s == null)
                {
                    throw new Exception(book.errorMessage());
                }
                return s;
            }

            set
            {
                int result = xlSheetSetFooter(handle, value, 0.5);
                if (result == 0)
                {
                    throw new Exception(book.errorMessage());
                }
            }
        }

        public double footerMargin
        {
            get
            {
                return xlSheetFooterMargin(handle);
            }

            set
            {
                int result = xlSheetSetFooter(handle, footer, value);
                if (result == 0)
                {
                    throw new Exception(book.errorMessage());
                }
            }
        }

        public bool hCenter
        {
            get { return xlSheetHCenter(handle) > 0; }
            set { xlSheetSetHCenter(handle, value ? 1 : 0); }
        }

        public bool vCenter
        {
            get { return xlSheetVCenter(handle) > 0; }
            set { xlSheetSetVCenter(handle, value ? 1 : 0); }
        }

        public double marginLeft
        {
            get { return xlSheetMarginLeft(handle); }
            set { xlSheetSetMarginLeft(handle, value); }
        }

        public double marginRight
        {
            get { return xlSheetMarginRight(handle); }
            set { xlSheetSetMarginRight(handle, value); }
        }

        public double marginTop
        {
            get { return xlSheetMarginTop(handle); }
            set { xlSheetSetMarginTop(handle, value); }
        }

        public double marginBottom
        {
            get { return xlSheetMarginBottom(handle); }
            set { xlSheetSetMarginBottom(handle, value); }
        }

        public bool printRowCol
        {
            get { return xlSheetPrintRowCol(handle) > 0; }
            set { xlSheetSetPrintRowCol(handle, value ? 1 : 0); }
        }

        public void setPrintRepeatRows(int rowFirst, int rowLast)
        {
            xlSheetSetPrintRepeatRows(handle, rowFirst, rowLast);
        }

        public void setPrintRepeatCols(int colFirst, int colLast)
        {
            xlSheetSetPrintRepeatCols(handle, colFirst, colLast);
        }

        public void setPrintArea(int rowFirst, int rowLast, int colFirst, int colLast)
        {
            xlSheetSetPrintArea(handle, rowFirst, rowLast, colFirst, colLast);
        }

        public void clearPrintRepeats()
        {
            xlSheetClearPrintRepeats(handle);
        }

        public void clearPrintArea()
        {
            xlSheetClearPrintArea(handle);
        }

        public void setNamedRange(string name, int rowFirst, int rowLast, int colFirst, int colLast)
        {
            int result = xlSheetSetNamedRange(handle, name, rowFirst, rowLast, colFirst, colLast);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public void delNamedRange(string name)
        {
            int result = xlSheetDelNamedRange(handle, name);
            if (result == 0)
            {
                throw new Exception(book.errorMessage());
            }
        }

        public string name
        {
            get { return xlSheetName(handle); }
            set { xlSheetSetName(handle, value); }
        }

        public bool protect
        {
            get { return xlSheetProtect(handle) > 0; }
            set { xlSheetSetProtect(handle, value ? 1 : 0); }
        }

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetCellType(IntPtr handle, int row, int col);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr xlSheetCellFormat(IntPtr handle, int row, int col);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetCellFormat(IntPtr handle, int row, int col, IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetIsFormula(IntPtr handle, int row, int col);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern string xlSheetReadStr(IntPtr handle, int row, int col, ref IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetWriteStr(IntPtr handle, int row, int col, string value, IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern double xlSheetReadNum(IntPtr handle, int row, int col, ref IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetWriteNum(IntPtr handle, int row, int col, double value, IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetReadBool(IntPtr handle, int row, int col, ref IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetWriteBool(IntPtr handle, int row, int col, int value, IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetReadBlank(IntPtr handle, int row, int col, ref IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetWriteBlank(IntPtr handle, int row, int col, IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern string xlSheetReadFormula(IntPtr handle, int row, int col, ref IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetWriteFormula(IntPtr handle, int row, int col, string value, IntPtr formatHandle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern string xlSheetReadComment(IntPtr handle, int row, int col);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetWriteComment(IntPtr handle, int row, int col, string value, string author, int width, int height);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetIsDate(IntPtr handle, int row, int col);
        
        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetReadError(IntPtr handle, int row, int col);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern double xlSheetColWidth(IntPtr handle, int col);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern double xlSheetRowHeight(IntPtr handle, int row);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetSetCol(IntPtr handle, int colFirst, int colLast, double width, IntPtr formatHandle, int hidden);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetSetRow(IntPtr handle, int row, double height, IntPtr formatHandle, int hidden);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetGetMerge(IntPtr handle, int row, int col, ref int rowFirst, ref int rowLast, ref int colFirst, ref int colLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetSetMerge(IntPtr handle, int rowFirst, int rowLast, int colFirst, int colLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetDelMerge(IntPtr handle, int row, int col);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetPicture(IntPtr handle, int row, int col, int pictureId, double scale);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetPicture2(IntPtr handle, int row, int col, int pictureId, int width, int height);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetSetHorPageBreak(IntPtr handle, int row, int pageBreak);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetSetVerPageBreak(IntPtr handle, int col, int pageBreak);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSplit(IntPtr handle, int row, int col);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSplit2(IntPtr handle, int x, int y, int rowTop, int colLeft, int frozen, int paneType);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetGroupRows(IntPtr handle, int rowFirst, int rowLast, int collapsed);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetGroupCols(IntPtr handle, int colFirst, int colLast, int collapsed);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetGroupSummaryBelow(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetGroupSummaryBelow(IntPtr handle, int below);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetGroupSummaryRight(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetGroupSummaryRight(IntPtr handle, int right);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetClear(IntPtr handle, int rowFirst, int rowLast, int colFirst, int colLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetInsertCol(IntPtr handle, int colFirst, int colLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetInsertRow(IntPtr handle, int rowFirst, int rowLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetRemoveCol(IntPtr handle, int colFirst, int colLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetRemoveRow(IntPtr handle, int rowFirst, int rowLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetCopyCell(IntPtr handle, int rowSrc, int colSrc, int rowDst, int colDst);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetFirstRow(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetLastRow(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetFirstCol(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetLastCol(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetDisplayGridlines(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetDisplayGridlines(IntPtr handle, int show);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetPrintGridlines(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetPrintGridlines(IntPtr handle, int print);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetZoom(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetZoom(IntPtr handle, int zoom);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetPrintZoom(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetPrintZoom(IntPtr handle, int zoom);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetLandscape(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetLandscape(IntPtr handle, int landscape);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetPaper(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetPaper(IntPtr handle, int paper);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern string xlSheetHeader(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetSetHeader(IntPtr handle, string header, double margin);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern double xlSheetHeaderMargin(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern string xlSheetFooter(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetSetFooter(IntPtr handle, string footer, double margin);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern double xlSheetFooterMargin(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetHCenter(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetHCenter(IntPtr handle, int hCenter);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetVCenter(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetVCenter(IntPtr handle, int vCenter);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern double xlSheetMarginLeft(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetMarginLeft(IntPtr handle, double margin);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern double xlSheetMarginRight(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetMarginRight(IntPtr handle, double margin);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern double xlSheetMarginTop(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetMarginTop(IntPtr handle, double margin);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern double xlSheetMarginBottom(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetMarginBottom(IntPtr handle, double margin);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetPrintRowCol(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetPrintRowCol(IntPtr handle, int print);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetPrintRepeatRows(IntPtr handle, int rowFirst, int rowLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetPrintRepeatCols(IntPtr handle, int colFirst, int colLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetPrintArea(IntPtr handle, int rowFirst, int rowLast, int colFirst, int colLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetClearPrintRepeats(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetClearPrintArea(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetSetNamedRange(IntPtr handle, string name, int rowFirst, int rowLast, int colFirst, int colLast);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetDelNamedRange(IntPtr handle, string name);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern string xlSheetName(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetName(IntPtr handle, string name);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int xlSheetProtect(IntPtr handle);

        [DllImport("libxl.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern void xlSheetSetProtect(IntPtr handle, int protect);
    }
}
