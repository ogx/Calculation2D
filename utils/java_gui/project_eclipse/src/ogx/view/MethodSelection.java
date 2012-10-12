package ogx.view;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;

import ogx.model.MethodModel;

public class MethodSelection implements Transferable {
	
	private final MethodModel current_method;
	private static final DataFlavor method_flavor = new DataFlavor(MethodModel.class, "MethodModel");
	private static final DataFlavor flavors[] = { method_flavor };
	
	public MethodSelection(MethodModel current_method) {
		this.current_method = current_method;
	}

	@Override
	public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException, IOException {
		if (isDataFlavorSupported(flavor)) {
			return current_method;
		}
		return null;
	}

	@Override
	public DataFlavor[] getTransferDataFlavors() {
		return flavors;
	}

	@Override
	public boolean isDataFlavorSupported(DataFlavor flavor) {
		if (flavor.equals(method_flavor)) return true;
		return false;
	}

}
