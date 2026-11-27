#!/usr/bin/env python3
"""Raw-colour lint: python3 scripts/check_raw_colors.py [--root DIR]

Screens should take colours from the theme (Theme::Usage, raised_surface(),
control_border(), subtle_border(), or shapes.h helpers), not invent them
inline. This flags NEW raw `Color{...}` literals in src/systems/screens/:
each file may keep exactly the count it has today, listed below with its
reason. Growing a count fails; shrinking one also fails until the allowlist
is lowered to match, so the total only ever ratchets down. A file not listed
may have none. Theme definitions (src/theme_presets.h) and shapes.h live
outside screens/ and are the places colours are supposed to be named.
"""
import pathlib, re, sys
root = pathlib.Path(sys.argv[sys.argv.index("--root")+1]) if "--root" in sys.argv else pathlib.Path(__file__).resolve().parent.parent
PATTERN = re.compile(r"(?:afterhours::)?Color\s*\{")
ALLOWLIST = {
    "AIMChatDemo.h": (9, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AbsolutePositioningShowcase.h": (18, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AdaptiveScalingDemo.h": (16, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AlphaBlendRepro.h": (5, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AngryBirdsSettings.h": (18, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AnimationBasicDemo.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AnimationDeclarativeDemo.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AnimationLoopingDemo.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AnimationSpringDemo.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AnimationStressLab.h": (5, "grandfathered raw colours in this screen; shrink, do not grow"),
    "AutoTextColorShowcase.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "BlendModeLab.h": (6, "grandfathered raw colours in this screen; shrink, do not grow"),
    "Buttons.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "Cards.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "CasualSettings.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ChartLab.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "CommandPickerDemo.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ComposerLab.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ContextMenuLab.h": (8, "grandfathered raw colours in this screen; shrink, do not grow"),
    "CozyCafe.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "CurvedTextLab.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "CurvedTextStressLab.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DeadSpaceSettings.h": (28, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DecorativeFrameShowcase.h": (7, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DecoratorShowcase.h": (7, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DialogBackdrop.h": (8, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DialogFyiShowcase.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DialogPresentation.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DialogPromptShowcase.h": (5, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DividerShowcase.h": (5, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DragDropShowcase.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "DropdownShowcase.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "EmpireTycoon.h": (30, "grandfathered raw colours in this screen; shrink, do not grow"),
    "EntityIndexLab.h": (8, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleAccessibility.h": (7, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleBevelBorders.h": (6, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleBorders.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleColorSwatch.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleColors.h": (6, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleFlexAlignment.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleLayout.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleNineSliceBorders.h": (7, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleText.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleTextOverflow.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleTextShadow.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ExampleTextStroke.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "FighterMenu.h": (25, "grandfathered raw colours in this screen; shrink, do not grow"),
    "FitContentLab.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "FlightOptions.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "FontWeightShowcase.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "Forms.h": (7, "grandfathered raw colours in this screen; shrink, do not grow"),
    "GridLab.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "GuessWhoLab.h": (18, "grandfathered raw colours in this screen; shrink, do not grow"),
    "HStackShowcase.h": (8, "grandfathered raw colours in this screen; shrink, do not grow"),
    "HoverLab.h": (11, "grandfathered raw colours in this screen; shrink, do not grow"),
    "IslandsTrainsSettings.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "KartSelect.h": (33, "grandfathered raw colours in this screen; shrink, do not grow"),
    "KirbyOptions.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "LanguageDemo.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "LayoutAcidTest.h": (5, "grandfathered raw colours in this screen; shrink, do not grow"),
    "LayoutBugRepros.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "LayoutPatterns.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "LocalizedInputPrompts.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "MarloKart.h": (16, "grandfathered raw colours in this screen; shrink, do not grow"),
    "MediaLibraryGrid.h": (10, "grandfathered raw colours in this screen; shrink, do not grow"),
    "MenuShowcase.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "MetersGaugesShowcase.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "MinesweeperLab.h": (30, "grandfathered raw colours in this screen; shrink, do not grow"),
    "MiniMotorwaysSettings.h": (16, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ModalAdvanced.h": (16, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ModalShowcase.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "MultiLineTextLab.h": (5, "grandfathered raw colours in this screen; shrink, do not grow"),
    "OffsiteBackupApp.h": (40, "grandfathered raw colours in this screen; shrink, do not grow"),
    "PaginationShowcase.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ParcelCorpsSettings.h": (17, "grandfathered raw colours in this screen; shrink, do not grow"),
    "PinchZoomLab.h": (10, "grandfathered raw colours in this screen; shrink, do not grow"),
    "PodcastLibrary.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "PopoverLab.h": (8, "grandfathered raw colours in this screen; shrink, do not grow"),
    "PotionCrafting.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "PowerWashSettings.h": (23, "grandfathered raw colours in this screen; shrink, do not grow"),
    "PseudoLocaleLab.h": (16, "grandfathered raw colours in this screen; shrink, do not grow"),
    "RaceResults.h": (11, "grandfathered raw colours in this screen; shrink, do not grow"),
    "RealWorldModals.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "RectAlgebraLab.h": (8, "grandfathered raw colours in this screen; shrink, do not grow"),
    "RollingNumberLab.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "RubberBanditsMenu.h": (25, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ScrollClickBug.h": (10, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ScrollClipBugRepro.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ScrollViewShowcase.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ScrollbarStyleLab.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "SecureTunnelMockup.h": (65, "grandfathered raw colours in this screen; shrink, do not grow"),
    "SettingRowShowcase.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "ShopInterface.h": (32, "grandfathered raw colours in this screen; shrink, do not grow"),
    "SplitPaneLab.h": (6, "grandfathered raw colours in this screen; shrink, do not grow"),
    "SportsSettings.h": (10, "grandfathered raw colours in this screen; shrink, do not grow"),
    "StepperShowcase.h": (6, "grandfathered raw colours in this screen; shrink, do not grow"),
    "StyledTextLab.h": (5, "grandfathered raw colours in this screen; shrink, do not grow"),
    "SyncScrollLab.h": (11, "grandfathered raw colours in this screen; shrink, do not grow"),
    "SystemProfileLab.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "TerminalDemo.h": (4, "grandfathered raw colours in this screen; shrink, do not grow"),
    "TextAreaLab.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "TextAreaStatesLab.h": (3, "grandfathered raw colours in this screen; shrink, do not grow"),
    "TouchTargetDemo.h": (1, "grandfathered raw colours in this screen; shrink, do not grow"),
    "TransitionsLab.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "TransparentControlsLab.h": (6, "grandfathered raw colours in this screen; shrink, do not grow"),
    "VirtualListHeightsLab.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "VirtualListLab.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
    "WidgetGapGallery.h": (2, "grandfathered raw colours in this screen; shrink, do not grow"),
}
bad = []
screens = root / "src/systems/screens"
for path in sorted(screens.glob("*.h")):
    text = path.read_text(errors="ignore")
    count = len(PATTERN.findall(text))
    allowed, reason = ALLOWLIST.get(path.name, (0, "no raw colours allowed in a new screen; use theme tokens"))
    if count > allowed:
        lines = [text.count("\n", 0, m.start()) + 1 for m in PATTERN.finditer(text)]
        bad.append(f"{path.name}: {count} raw Color{{...}} literals, allowlisted {allowed} ({reason}) at lines {lines} -- fix: use a theme token (Theme::Usage / raised_surface / control_border / subtle_border / shapes::card) or, for intentional game-art colour, raise the count here with a reason")
    elif count < allowed:
        bad.append(f"{path.name}: {count} raw Color{{...}} literals, allowlisted {allowed} ({reason}) -- fix: lower the allowlist in scripts/check_raw_colors.py to {count} so the saving is banked")
for name in ALLOWLIST:
    if not (screens / name).exists():
        bad.append(f"{name}: allowlisted but the file is gone -- fix: delete its entry from scripts/check_raw_colors.py")
print("\n".join(bad) if bad else f"check_raw_colors: OK ({sum(v[0] for v in ALLOWLIST.values())} grandfathered literals in {len(ALLOWLIST)} files, none new)")
sys.exit(1 if bad else 0)
