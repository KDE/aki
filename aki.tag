<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="class">
    <name>NetworkList</name>
    <filename>classNetworkList.html</filename>
    <member kind="typedef">
      <type>QList&lt; NetworkListItem * &gt;</type>
      <name>List</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>af63bf678aaabca0400c1f33c5656e31d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NetworkList</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>afb2b92a42667a5429ad8eec15c9f16da</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~NetworkList</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>abb2901fa49f2515ac8faf121608f3d7c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addNetwork</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>adcba14c0b3f31dcb519372013d758a90</anchor>
      <arglist>(NetworkListItem *network)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addNetworks</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>abbbeff9da2d3dd6d3090fd8475cb8b1a</anchor>
      <arglist>(const NetworkList::List &amp;list)</arglist>
    </member>
    <member kind="function">
      <type>NetworkListItem *</type>
      <name>currentItem</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>ad5fee5313c3242869fd239c90eefbd16</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>currentRow</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>a25b5a259ae90187a6805226b0fb90f0c</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>NetworkList::List</type>
      <name>findItems</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>a689f5e079e5f4922edd9ccc73aab4b11</anchor>
      <arglist>(const QString &amp;name, Qt::MatchFlags flags) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>insertNetwork</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>adc08c1685e619a15003d07ffbaeed679</anchor>
      <arglist>(int row, NetworkListItem *network)</arglist>
    </member>
    <member kind="function">
      <type>NetworkListItem *</type>
      <name>item</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>ac4c6766712a2068ad71ba836731f1a1b</anchor>
      <arglist>(int row) const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>row</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>a9df4d661a12456a81189f7a7f13ce4f3</anchor>
      <arglist>(NetworkListItem *item) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCurrentNetwork</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>ab3a314600de7f7aff7082c7235654781</anchor>
      <arglist>(NetworkListItem *network, QItemSelectionModel::SelectionFlags command)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCurrentNetwork</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>afebed464c7d972d0ef91dbb7a5db1b1d</anchor>
      <arglist>(NetworkListItem *network)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCurrentRow</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>a000c1d8f6ee51267240ccf687b6a12ca</anchor>
      <arglist>(int row, QItemSelectionModel::SelectionFlags command)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCurrentRow</name>
      <anchorfile>classNetworkList.html</anchorfile>
      <anchor>a9fe2161cbc6f4557a98a866cfa60e97f</anchor>
      <arglist>(int row)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Aki::ChatViewPrivate</name>
    <filename>classAki_1_1ChatViewPrivate.html</filename>
    <member kind="function">
      <type>QDomElement</type>
      <name>awayMessage</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>a83ba1e6993d1d8618a99610245fda9a3</anchor>
      <arglist>(const QString &amp;message)</arglist>
    </member>
    <member kind="function">
      <type>QDomElement</type>
      <name>ban</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>a6c639e150eb9d480bce369cb118bfe27</anchor>
      <arglist>(const QString &amp;mask)</arglist>
    </member>
    <member kind="function">
      <type>QDomElement</type>
      <name>by</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>a2e438b3e5dcf0db311839597b9fb7dd0</anchor>
      <arglist>(const Aki::Irc::NickInfo &amp;sender, const QString &amp;klass=QString(), bool self=false)</arglist>
    </member>
    <member kind="function">
      <type>QDomElement</type>
      <name>createEvent</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>afa82b2ff68d44038349b463f7e2763df</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>QDomElement</type>
      <name>envelope</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>a6054f3f9270cc18f9c6f16aaf65da667</anchor>
      <arglist>(bool ignored=false)</arglist>
    </member>
    <member kind="function">
      <type>QDomElement</type>
      <name>log</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>a95d83abccfc7ed281b23fc0619791d51</anchor>
      <arglist>(const QString &amp;began, const QString &amp;source)</arglist>
    </member>
    <member kind="function">
      <type>QDomElement</type>
      <name>message</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>ae4beef6f5789584dad728d2509e14d08</anchor>
      <arglist>(const QString &amp;msg, bool highlight=false, bool action=false, bool notice=false)</arglist>
    </member>
    <member kind="function">
      <type>QDomElement</type>
      <name>old</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>ab57976fdd20eeab18949b3c7e2b40c6f</anchor>
      <arglist>(const QString &amp;oldNick)</arglist>
    </member>
    <member kind="function">
      <type>QDomElement</type>
      <name>reason</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>adff6d4f549c100bd4e1aff6846a9aa5d</anchor>
      <arglist>(const QString &amp;reason=QString())</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>span</name>
      <anchorfile>classAki_1_1ChatViewPrivate.html</anchorfile>
      <anchor>a1193e46535904d27b6ed0e5ccd12a2cb</anchor>
      <arglist>(const QString &amp;klass, const QString &amp;text)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Aki::ConfigFile</name>
    <filename>classAki_1_1ConfigFile.html</filename>
    <member kind="function">
      <type></type>
      <name>ConfigFile</name>
      <anchorfile>classAki_1_1ConfigFile.html</anchorfile>
      <anchor>acb81016efe70a9f923f2e494184c9f41</anchor>
      <arglist>(const QString &amp;name, QObject *parent=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~ConfigFile</name>
      <anchorfile>classAki_1_1ConfigFile.html</anchorfile>
      <anchor>ae73765f7f0320bb6422da1cebe866f31</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>createGroup</name>
      <anchorfile>classAki_1_1ConfigFile.html</anchorfile>
      <anchor>aee62079398d567b94279a5931f968ef4</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>KConfigGroup</type>
      <name>currentGroup</name>
      <anchorfile>classAki_1_1ConfigFile.html</anchorfile>
      <anchor>a0c026184455d5bd800a831a096613e49</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>deleteGroup</name>
      <anchorfile>classAki_1_1ConfigFile.html</anchorfile>
      <anchor>a9d17f30f47301b7293bcdcd8d07e93b4</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>QStringList</type>
      <name>groupLists</name>
      <anchorfile>classAki_1_1ConfigFile.html</anchorfile>
      <anchor>a8500529730593fe79b5e11907a5b52d3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>save</name>
      <anchorfile>classAki_1_1ConfigFile.html</anchorfile>
      <anchor>ab13acdfb1472f6672574a30e7ae1ba2a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setCurrentGroup</name>
      <anchorfile>classAki_1_1ConfigFile.html</anchorfile>
      <anchor>a84b466fcc804515a017b40d885c08321</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Aki::DockBar</name>
    <filename>classAki_1_1DockBar.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>dockAdded</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>a54a68928870c4187caafcedcdb1529b3</anchor>
      <arglist>(Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>dockHide</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>a06f8466fd4341aa3f7f189692890a83e</anchor>
      <arglist>(Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>dockRemoved</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>a90704176902ec8a1753252df8437cce3</anchor>
      <arglist>(Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>dockShow</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>a24a66a961240e54b7c209ab39e2d33b1</anchor>
      <arglist>(Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DockBar</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>a9a7d0704d0334a9cb0915acd9728966e</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~DockBar</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>a8ba881ea7240d211a84005cd5bd7ee75</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addDock</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>a64671375aaf6925301fef0ebf867ae18</anchor>
      <arglist>(Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="function">
      <type>Qt::ToolBarAreas</type>
      <name>area</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>a983b1a9c2a8aff541a2a6385972e187d</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Aki::DockButton *</type>
      <name>buttonForAction</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>af75510cbae3ea6cebb8f5f0698aeb14d</anchor>
      <arglist>(QAction *action)</arglist>
    </member>
    <member kind="function">
      <type>Aki::DockButton *</type>
      <name>buttonForDock</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>adb5b8332683ae30e7f4836b1202a0ad0</anchor>
      <arglist>(Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>removeDock</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>aeb3322450bc12eadebf68dc95b4d6751</anchor>
      <arglist>(Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDockArea</name>
      <anchorfile>classAki_1_1DockBar.html</anchorfile>
      <anchor>a2eda807085e4da4aaf62c87aaaf6d456</anchor>
      <arglist>(Qt::ToolBarArea area)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Aki::DockButton</name>
    <filename>classAki_1_1DockButton.html</filename>
    <member kind="function">
      <type></type>
      <name>DockButton</name>
      <anchorfile>classAki_1_1DockButton.html</anchorfile>
      <anchor>a9f6df8e8671d91703fd9e632e6e62083</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~DockButton</name>
      <anchorfile>classAki_1_1DockButton.html</anchorfile>
      <anchor>abc03c2b5c063213517333c143c23f5a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Aki::DockWidget *</type>
      <name>dock</name>
      <anchorfile>classAki_1_1DockButton.html</anchorfile>
      <anchor>a279fe3c9714602446b081aee50eb57b8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Aki::DockWidget *</type>
      <name>dock</name>
      <anchorfile>classAki_1_1DockButton.html</anchorfile>
      <anchor>abfb2a9a1cc159e3d19fae0099694a410</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isAutoHide</name>
      <anchorfile>classAki_1_1DockButton.html</anchorfile>
      <anchor>a59c2b7f6eae994d9e73adf30361f80f7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Qt::Orientation</type>
      <name>orientation</name>
      <anchorfile>classAki_1_1DockButton.html</anchorfile>
      <anchor>a443859623cd77635146ea16f9a17e12b</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setAutoHide</name>
      <anchorfile>classAki_1_1DockButton.html</anchorfile>
      <anchor>a521fd163616dda64b4276eff5e1b2a3a</anchor>
      <arglist>(bool hideable)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDock</name>
      <anchorfile>classAki_1_1DockButton.html</anchorfile>
      <anchor>a13a2559e89e0cea68384841903e07424</anchor>
      <arglist>(Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setOrientation</name>
      <anchorfile>classAki_1_1DockButton.html</anchorfile>
      <anchor>acf44146a5ad9ba08d520bf7ed92eb43b</anchor>
      <arglist>(Qt::Orientation orientation)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Aki::DockWidget</name>
    <filename>classAki_1_1DockWidget.html</filename>
    <member kind="signal">
      <type>void</type>
      <name>dockAutoHideStateChanged</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>a90d9dca00503bfbec245ee3b712a51f6</anchor>
      <arglist>(bool checked)</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>mouseLeave</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>a2fbaa28e62146613701686fd35d105a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="signal">
      <type>void</type>
      <name>titleChanged</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>ac44aeaaf141565a56718ca8c3e23ea63</anchor>
      <arglist>(const QString &amp;title)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DockWidget</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>a9a689a33186a3c911d5a0d416199d371</anchor>
      <arglist>(const QString &amp;title, const KIcon &amp;icon=KIcon(), QWidget *parent=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DockWidget</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>af213917554bcdedd98c2d36385a8ecbe</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~DockWidget</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>a7b2b516bd3345ea04ff6d3ff57b219b9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Aki::DockBar *</type>
      <name>dockBar</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>ae8bf8a1ab922015fc0d118df1fa50174</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Aki::DockBar *</type>
      <name>dockBar</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>a6c5229ed50cc78446ba0c6e03fc91c8a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>KIcon</type>
      <name>icon</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>a02dcfc42d4f747a0496ffab50de22c11</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDockBar</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>ab9a04e9ed6883394a233b30b3efc81b5</anchor>
      <arglist>(Aki::DockBar *dockBar)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setIcon</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>a17846165d57881413fa83055a547757e</anchor>
      <arglist>(const KIcon &amp;icon)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setTitle</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>a11e255e03bfea26a74bbf0fe2ecb1360</anchor>
      <arglist>(const QString &amp;title)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>title</name>
      <anchorfile>classAki_1_1DockWidget.html</anchorfile>
      <anchor>a96592af20b2072b3f072eed57c3085cc</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Aki::MainWindow</name>
    <filename>classAki_1_1MainWindow.html</filename>
    <member kind="function">
      <type></type>
      <name>MainWindow</name>
      <anchorfile>classAki_1_1MainWindow.html</anchorfile>
      <anchor>a34c4b4207b46d11a4100c9b19f0e81bb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~MainWindow</name>
      <anchorfile>classAki_1_1MainWindow.html</anchorfile>
      <anchor>ae98d00a93bc118200eeef9f9bba1dba7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addDock</name>
      <anchorfile>classAki_1_1MainWindow.html</anchorfile>
      <anchor>aa3add12a89e76425bfca443cd95da14e</anchor>
      <arglist>(Qt::ToolBarArea area, Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>removeDock</name>
      <anchorfile>classAki_1_1MainWindow.html</anchorfile>
      <anchor>aa2036d0fdfd31152b87dfa0c65e0d9a4</anchor>
      <arglist>(Aki::DockWidget *dock)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setView</name>
      <anchorfile>classAki_1_1MainWindow.html</anchorfile>
      <anchor>a0f7234c5d9fc134c17c6a23bc0c73f6c</anchor>
      <arglist>(Aki::View *view)</arglist>
    </member>
    <member kind="function">
      <type>Aki::ViewTabBar *</type>
      <name>topRightCorner</name>
      <anchorfile>classAki_1_1MainWindow.html</anchorfile>
      <anchor>ac8ec734227a592e5703b22faaa25a361</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Aki::ViewTabBar *</type>
      <name>topRightCorner</name>
      <anchorfile>classAki_1_1MainWindow.html</anchorfile>
      <anchor>a224396282f27ad36dcdaff48260108d7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>setTopRightCorner</name>
      <anchorfile>classAki_1_1MainWindow.html</anchorfile>
      <anchor>a1562aba473c1630fcfba5492ebadb857</anchor>
      <arglist>(Aki::ViewTabBar *widget)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Aki::SqlNickname</name>
    <filename>classAki_1_1SqlNickname.html</filename>
    <member kind="typedef">
      <type>QList&lt; Aki::SqlNickname * &gt;</type>
      <name>List</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>a2697df11120660186b407db0d3e1ded3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~SqlNickname</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>a92e3706ab32e39e327e193657d83577d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>id</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>a855ca271fb99178bbb13679600500265</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>nickname</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>a96507b13b62054be496f19dc4089182c</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>nicknameServerId</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>ad61c94a5a2c5f04a287101dcadcc45dc</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>aac8a386d5916b05bc5b12606d82aa41b</anchor>
      <arglist>(const Aki::SqlNickname &amp;rhs) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>a548ae79793df03a16f2067e3b764ed1b</anchor>
      <arglist>(const Aki::SqlNickname &amp;rhs) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>remove</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>aeb2fda8e391a1115291c7550d8eb0439</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>save</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>ac9655444b61eb477656ee23220452f07</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setNickname</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>a9683d3fd25c3ed0fdc1ac766e6d6bd15</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setNicknameServerId</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>aa28c2ce0f8363c37d61aae5bb0e4583e</anchor>
      <arglist>(int id)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Aki::SqlNickname *</type>
      <name>findNickname</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>aea148869ea39d3a688276353d8bee960</anchor>
      <arglist>(const QString &amp;nickname, const Aki::SqlServer *server)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Aki::SqlNickname *</type>
      <name>newNickname</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>afba0c5ed49075ee760029f90d4993a6c</anchor>
      <arglist>(const QString &amp;nickname, const Aki::SqlServer *server)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Aki::SqlNickname::List</type>
      <name>nicknamesForServer</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>af253f3a4f6dad222e5da8480d9519188</anchor>
      <arglist>(const Aki::SqlServer *server)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>SqlNickname</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>a300f8e9ab4652fb0bbf1d5679e0d629e</anchor>
      <arglist>(QObject *parent=0)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>setId</name>
      <anchorfile>classAki_1_1SqlNickname.html</anchorfile>
      <anchor>a89cc1a95c3e2f7c15a146e25a32dc2cf</anchor>
      <arglist>(int id)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Aki::SqlServer</name>
    <filename>classAki_1_1SqlServer.html</filename>
    <member kind="typedef">
      <type>QList&lt; Aki::SqlServer * &gt;</type>
      <name>List</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a90147b893874630a33605085222eb869</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~SqlServer</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a564c0ed33329f07178a1f9627966c29c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>QByteArray</type>
      <name>encoding</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a583facd54efccd012405123ca3e4d25c</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>id</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>ab90098a9c5586e2082fbb50fefe07b66</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isAutoIdentifyEnabled</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a898cd5231142238faa7789613fd0ba8e</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isAutoJoinChannelsEnabled</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a4750110fea3f60c5609d28d7794be4ea</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isAutoReconnectEnabled</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a0d0e71ba1aaa00f720dfa5b01701b5d5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConnectOnStartupEnabled</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a8d106249f9fb0872ceb4e1df648f5d3c</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConnectToRandomServerEnabled</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>ade7525d74aa08d0910b8cb0a0520d1d2</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isDefaultEncodingEnabled</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a7af84fdebab8e0d773d4bc5608dba252</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>name</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>adb4be8e5b09687ba4b61bb4ba160d7ab</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>ace6efd6ef1a344f6c0e58f8fb332fa49</anchor>
      <arglist>(const Aki::SqlServer &amp;rhs) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>adf1698a6922f61f17785f53298460728</anchor>
      <arglist>(const Aki::SqlServer &amp;rhs) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>remove</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a188c10b6776658e37bad397c220ab3a0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>retryAttemptCount</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>abd0bde1e119af4f68cdf4577c8149a16</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>retryInterval</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a9d97f9aec5ad0d85821134abebfde39f</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>save</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a3e7f736e0f7a5f5b3d32638314eb8016</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>serverIdentityId</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a867a8b4793d7e9681a2528805b61d83d</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Aki::SqlServer::List</type>
      <name>serversForIdentity</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a208217e521d44238ac52e38f4f8a4fb2</anchor>
      <arglist>(const Aki::SqlIdentity *identity)</arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>serviceName</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>ad438114acf5d4693c9b1d491625178cb</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>QString</type>
      <name>servicePassword</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a41a30f45e16f68311c940e7edc776147</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setAutoIdentity</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a01a1e8d8b2fba15d138f138141bfefa1</anchor>
      <arglist>(bool enabled)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setAutoJoinChannels</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>ae8c6e46721b267aecb4b11e90247bf1f</anchor>
      <arglist>(bool enabled)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setAutoReconnect</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a5c4b14665e4af9525e08b6302e1a66d1</anchor>
      <arglist>(bool enabled)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setConnectOnStartup</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>ab7078b9d32e91f2d73649fb346f98b9c</anchor>
      <arglist>(bool enabled)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setConnectToRandomServer</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a4da088ef207359cbfcf897c23ceed49f</anchor>
      <arglist>(bool enabled)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDefaultEncoding</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a8424f17f4bf3a6aefdecc3688fd020e5</anchor>
      <arglist>(bool enabled)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setEncoding</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a6fe770c5107c646cd2df4294545c03f2</anchor>
      <arglist>(const QByteArray &amp;encoding)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setName</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a9b99456527a7af1c551bab306c30ddca</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setRetryAttempts</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>aa87d2b24720696cfa92d3cdb795b18b1</anchor>
      <arglist>(int count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setRetryInterval</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a5a9ef42fe587e188b372030035ff8a8b</anchor>
      <arglist>(int seconds)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setServerIdentityId</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>acde9cd51c70323a61915426aa6291553</anchor>
      <arglist>(int id)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setServiceName</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a4f76599cf8e00dab16a3aceb5da2bc5c</anchor>
      <arglist>(const QString &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setServicePassword</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a3693fdfeb49f777a73a04aa0f1f42533</anchor>
      <arglist>(const QString &amp;password)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Aki::SqlServer *</type>
      <name>findServer</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a86339b86d684c6fa0c743b00e53210ff</anchor>
      <arglist>(const QString &amp;name, const Aki::SqlIdentity *identity)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Aki::SqlServer *</type>
      <name>newServer</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a6909995ef1f952118a41fd5fef227c7e</anchor>
      <arglist>(const QString &amp;name, const Aki::SqlIdentity *identity)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type></type>
      <name>SqlServer</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>acc5aaa8f4be38be3cc9f675b6f06e7ec</anchor>
      <arglist>(QObject *parent=0)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>setId</name>
      <anchorfile>classAki_1_1SqlServer.html</anchorfile>
      <anchor>a2e2995d47287ad5be05a899fdf32fc5c</anchor>
      <arglist>(int id)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Aki::View</name>
    <filename>classAki_1_1View.html</filename>
    <member kind="slot" virtualness="virtual">
      <type>virtual void</type>
      <name>viewCurrentChanged</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>a1519b18109dce7f813726d1d5c4459cc</anchor>
      <arglist>(int index)</arglist>
    </member>
    <member kind="slot" virtualness="virtual">
      <type>virtual void</type>
      <name>viewPositionChanged</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>ae3a1951f379c32c1fe7cadc60dc896e2</anchor>
      <arglist>(int oldIndex, int newIndex)</arglist>
    </member>
    <member kind="slot" virtualness="virtual">
      <type>virtual void</type>
      <name>viewPositionClose</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>a6948229721d99274503ce4b7113db41c</anchor>
      <arglist>(int index)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>View</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>a74d64dd513eae96bc3196131f865a4e8</anchor>
      <arglist>(QWidget *parent=0)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~View</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>ad0dc854db9aabbea98a334dec89f785c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addWindow</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>a860e48aaa5c0235360c9d20ff2417281</anchor>
      <arglist>(Aki::IBaseWindow *window)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>count</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>a656ad83266edd36a05959718ddb6edd0</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>Aki::IBaseWindow *</type>
      <name>currentWindow</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>a51a181166b901df1b8aabdb72ee28aae</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>indexOf</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>abd1c1b888a7d938f08b87c74f27d5e49</anchor>
      <arglist>(Aki::IBaseWindow *window)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>insertWindow</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>a9cc5dd36746c55ba089f21b6d86be165</anchor>
      <arglist>(int index, Aki::IBaseWindow *window)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>removeWindow</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>aa2aa0af30bca356e8613beae96377dc4</anchor>
      <arglist>(Aki::IBaseWindow *window)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCurrentIndex</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>a976ac68bdf55e97413c4bd15a5cad833</anchor>
      <arglist>(int index)</arglist>
    </member>
    <member kind="function">
      <type>Aki::IBaseWindow *</type>
      <name>window</name>
      <anchorfile>classAki_1_1View.html</anchorfile>
      <anchor>a2baf6f3ff4fb10844eaf252deaf0b99f</anchor>
      <arglist>(int index) const </arglist>
    </member>
  </compound>
</tagfile>
