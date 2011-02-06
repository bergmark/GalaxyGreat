//==================================================================================================
// Native Function Prototypes
//==================================================================================================
include "TriggerLibs/GameData/GameDataAllNatives.galaxy"

//--------------------------------------------------------------------------------------------------
// Contents
//--------------------------------------------------------------------------------------------------
// - About Types
// - Common
// - Achievements
// - Actor
// - Animation
// - Bank
// - Boards
// - Camera
// - Campaign
// - Catalogs
// - Cinematics
// - Conversions
// - Conversation
// - Data Table
// - Dialogs
// - Environment
// - Game
// - Looping
// - Markers
// - Math
// - Melee
// - Minimap
// - Movie
// - Objectives
// - Orders
// - Ping
// - Planet
// - Players
// - Player Groups
// - Points
// - Portraits
// - Preload
// - PurchaseItems
// - Regions
// - Sound
// - Story Mode
// - Strings
// - Tech Tree
// - Text Tags
// - Timing
// - Transmission
// - Triggers
// - Units
// - Unit Filters
// - Unit Groups
// - Unit Reference
// - Unit Selection
// - User Interface
// - Visibility
//
//--------------------------------------------------------------------------------------------------
// About Types
//--------------------------------------------------------------------------------------------------
//
// -- Complex types and automatic deletion --
//
// Many native types represent "complex" objects (i.e. larger than 4 bytes).  The script language
// automatically keeps track of these objects and deletes them from memory when they are no longer
// used (that is, when nothing in the script references them any longer).  The types which benefit
// from automatic deletion are:
//
//      abilcmd, bank, camerainfo, marker, order, playergroup, point,
//      region, soundlink, string, text, timer, transmissionsource, unitfilter, unitgroup, unitref,
//      waveinfo, wavetarget
//
// Other object types must be explicitly destroyed with the appropriate native function when you
// are done using them.
//
//
// -- Complex types and equality --
//
// Normally, comparing two "complex" objects with the == or != operators will only compare the
// object reference, not the contained object data.  However, a few types will compare the contained
// data instead.  These types are:
//
//      abilcmd, point, string, unitfilter, unitref
//
// Examples:
//
//      Point(1, 2) == Point(1, 2)                              // True
//      "test string" == "test string"                          // True (note: this is case sensitive)
//      AbilityCommand("move", 0) == AbilityCommand("move", 0)  // True
//      Order(abilCmd) == Order(abilCmd)                        // False (two different order instances)
//      RegionEmpty() == RegionEmpty()                          // False (two different region instances)
//
//
// -- Complex types and +/- operators --
//
// Besides numerical types (byte, int, fixed), a few complex types support + and/or - operators:
//
//      string, text    + operator will concatenate the strings or text
//      point           +/- operators will add or subtract the x and y components of the points
//

//--------------------------------------------------------------------------------------------------
// Common
// - Constants used in multiple sections
//--------------------------------------------------------------------------------------------------
// Alignment
const int c_alignLeft   = 0;
const int c_alignTop    = 0;
const int c_alignRight  = 1;
const int c_alignBottom = 1;
const int c_alignCenter = 2;

// Anchor
const int c_anchorTopLeft       = 0;
const int c_anchorTop           = 1;
const int c_anchorTopRight      = 2;
const int c_anchorLeft          = 3;
const int c_anchorCenter        = 4;
const int c_anchorRight         = 5;
const int c_anchorBottomLeft    = 6;
const int c_anchorBottom        = 7;
const int c_anchorBottomRight   = 8;

//--------------------------------------------------------------------------------------------------
// Achievements
//--------------------------------------------------------------------------------------------------
native void AchievementPanelSetVisible (playergroup players, bool visible);
native void AchievementsDisable (int player);

//--------------------------------------------------------------------------------------------------
// Actor
// - Can send actor messages through API that is temporarily similar to the animation API.
//
// Design Comments
// - Trigger code is synchronous and actors are asynchronous.  This means we can't have any
//   routines that return any results, since anything from the actor system is suspect.
//   This includes even the existence of actors themselves; an actor might only be created on
//   machines with good graphics cards, for instance.
// - This also means collection classes are out, since we can't return a count that is guaranteed
//   to match.  (Technically, we could still have them, but just not populate them via "query".
//   The value at this point is unfortunately low enough to not be worth the complexity.)
// - We can return actors and scopes, but these will be opaque values that might even be NULL.
//   The client can do stuff with them, but won't be guaranteed to have them work.  In other
//   words, the client always appears to get a valid object, but the user has no way of actually
//   knowing whether it was valid, except to look at the screen.  Testing against null only
//   determines whether there has ever been an attempt to assign these values.
//
// FAQ
//
// Q01  Why do we not use scope names to get a hold of scopes?
// A01  Scope names started as a debugging tool.  We want to see if we can get by with that.
//      Until there's a super compelling need, we want to get by without exposing a whole new
//      namespace.
//
// Q02  Why does ::LastCreated only get set with new values if they are successfully created?
//      Won't this create subtle bugs where we send messages to the prior actor that was created,
//      b/c we are accidentally not creating a new one?
// A02  Well, it might.  But it's also a simply explained rule.  No one will remember the
//      alternative, which is something like actors/scopes created by ActorCreate, and create
//      messages sent by the user.  It also leads to simpler, less bug-prone C++ code on my end. :)
//
// Q03  Why does ActorCreate have 3 content params?  Is that many really necessary?
// A03  It is exactly enough to create a beam with a custom model and two custom sites.
//--------------------------------------------------------------------------------------------------
native actorscope   ActorScopeCreate (string name);
native actorscope   ActorScopeFrom (string name);
native actorscope   ActorScopeFromActor (actor a);
native actorscope   ActorScopeFromUnit (unit u);
native actorscope   ActorScopeFromPortrait (int p);
native text         ActorScopeGetText (actorscope as);
native void         ActorScopeKill (actorscope as);
native void         ActorScopeOrphan (actorscope as);
native actor        ActorScopeRefGet (actorscope as, string refName);
native void         ActorScopeRefSet (actorscope as, string refName, actor aValue);
native void         ActorScopeSend (actorscope as, string msg);
native actor        ActorCreate (actorscope as, string actorName, string content1Name, string content2Name, string content3Name);
native actor        ActorFrom (string name);
native actor        ActorFromActor (actor a, string name);
native actor        ActorFromScope (actorscope as, string name);
native actor        ActorFromDoodad (doodad d);
native actor        ActorFromPortrait (int p);
native text         ActorGetText (actor a);
native actor        ActorRefGet (actor a, string refName);
native void         ActorRefSet (actor a, string refName, actor aValue);
native void         ActorSend (actor a, string msg);
native void         ActorSendTo (actor a, string refName, string msg);
native void         ActorLookAtStart (actor s, string lookAt, int weight, fixed time, actor t);
native void         ActorLookAtStop (actor s, string lookAt, int weight, fixed time);
native void         ActorLookAtTypeStart (actor s, string type, actor t);
native void         ActorLookAtTypeStop (actor s, string type);
native void         ActorTextureGroupApplyGlobal (string textureProps);
native void         ActorTextureGroupRemoveGlobal (string textureProps);
native void         ActorWorldParticleFXDestroy ();

native actor        ActorRegionCreate (actorscope as, string actorName, region r);
native void         ActorRegionSend (actor a, int intersect, string msg, string filters, string terms);

const int c_actorIntersectAgainstCenter             = 0;
const int c_actorIntersectAgainstRadiusContact      = 1;
const int c_actorIntersectAgainstRadiusGame         = 2;
const int c_actorIntersectAgainstRadiusVisual       = 3;

const int c_animTimeVariantAsAutomatic              = 0;
const int c_animTimeVariantAsDuration               = 1;
const int c_animTimeVariantAsTimeScale              = 2;

const int c_animTransitionFlagInstant               = (1 << 0);

const int c_animBracketStartFlagClosingFull         = (1 << 0);
const int c_animBracketStartFlagContentNonLooping   = (1 << 1);
const int c_animBracketStartFlagContentPlayOnce     = (1 << 2);
const int c_animBracketStartFlagOpeningPlayForever  = (1 << 3);
const int c_animBracketStartFlagInstant             = (1 << 4);

const int c_animGroupApplyFlagClosingFull           = (1 << 0);
const int c_animGroupApplyFlagInstant               = (1 << 1);

const int c_animFlagFullMatch                       = (1 << 0);
const int c_animFlagPlayForever                     = (1 << 1);
const int c_animFlagNonLooping                      = (1 << 2);
const int c_animFlagAssetDrivenLooping              = (1 << 3);
const int c_animFlagRandomStartOffset               = (1 << 4);

// Currently unused.
const int e_actorRefFlagGroup                       = (1 << 1);

const int c_actorRefSpaceGlobal                     = 1;
const int c_actorRefSpaceScope                      = 2;
const int c_actorRefSpaceActor                      = 3;

const int c_actorRequestScopeImplicit               = 0;
const int c_actorRequestScopeCaster                 = 1;
const int c_actorRequestScopeEffect                 = 2;
const int c_actorRequestScopeMissile                = 3;
const int c_actorRequestScopeOuter                  = 4;
const int c_actorRequestScopeSource                 = 5;
const int c_actorRequestScopeTarget                 = 6;

const int c_actorRequestActorImplicit               = 0;
const int c_actorRequestActorCreate                 = 1;
const int c_actorRequestActorFind                   = 2;

const int c_textureVideoPlayFlagLooping             = (1 << 0);
const int c_textureVideoPlayFlagSynced              = (1 << 1);

const int c_physicsDisabled                         = 0;
const int e_physicsKeyframed                        = 1;
const int e_physicsSimulated                        = 2;

const int c_actorTransitionIn                       = 0;
const int c_actorTransitionOut                      = 1;

const int c_textureSlotComponentDefault             = 0;
const int c_textureSlotComponentDiffuse             = 1;
const int c_textureSlotComponentEmissive            = 2;
const int c_textureSlotComponentNormal              = 3;
const int c_textureSlotComponentSpecular            = 4;

native string       MakeMsgAnimBracketResume (
                        string animName,
                        int animTransitionFlags,
                        fixed timeVariant,
                        int timeType
                    );

native string       MakeMsgAnimBracketStart (
                        string animName,
                        string propsOpening,
                        string propsContent,
                        string propsClosing,
                        int animBracketStartFlags,
                        fixed timeVariant,
                        int timeType
                    );

native string       MakeMsgAnimBracketStop (
                        string animName,
                        int animTransitionFlags,
                        fixed timeVariant,
                        int timeType
                    );

native string       MakeMsgAnimGroupApply (
                        string animGroup,
                        string atApply,
                        string atRemove,
                        int animGroupApplyFlags,
                        fixed timeVariant,
                        int timeType
                    );

native string       MakeMsgAnimGroupRemove (
                        string animGroup,
                        int animTransitionFlags,
                        fixed timeVariant,
                        int timeType
                    );

native string       MakeMsgAnimPlay (
                        string animName,
                        string animProps,
                        int animFlags,
                        fixed blendIn,
                        fixed blendOut,
                        fixed timeVariant,
                        int timeType
                    );

native string       MakeMsgRefCreate (string refName);
native string       MakeMsgRefSetFromRequest (
                        string refName,
                        string subject,
                        string effectName,
                        int requestScope,
                        int requestActor
                    );
native string       MakeMsgRefTableDump (int space);
native string       MakeMsgSetPhysicsState (int physicsState, fixed delayLow, fixed delayHigh);
native string       MakeMsgTextureSelectByMatch (
                        string slotName,
                        int slotComponent,
                        string sourceSlotName,
                        int sourceSlotComponent
                    );
native string       MakeMsgTextureSelectBySlot (
                        string slotName,
                        int slotComponent,
                        string textureExpression
                    );
native string       MakeMsgTextureVideoPlay (
                        string slotName,
                         int slotComponent,
                         int fps,
                         int textureVideoPlayFlags,
                         int videoSoundType,
                         string attachQuery
                    );
native string       MakeMsgTextureVideoStop (string slotName, int slotComponent);
native string       MakeMsgTextureVideoSetFrame (string slotName, int slotComponent, int frame);
native string       MakeMsgTextureVideoSetPaused (string slotName, int slotComponent, bool isPaused);
native string       MakeMsgTextureVideoSetTime (string slotName, int slotComponent, fixed time);
native string       MakeMsgTransition (int transitionType, fixed durationBase, fixed durationRange);

native string       TextureGetSlotName (string textureLink);
native int          TextureGetSlotComponent (string textureLink);

native doodad       DoodadFromId (int id);

//--------------------------------------------------------------------------------------------------
// Animation
//--------------------------------------------------------------------------------------------------
const fixed c_animTimeDefault           = -1;

const string c_animNameDefault          = "Default";

native void     ModelAnimationLoad (string modelPath, string animPath);
native void     ModelAnimationUnload (string modelPath, string animPath);

//--------------------------------------------------------------------------------------------------
// Bank
//--------------------------------------------------------------------------------------------------
const int c_bankTypeFixed   = 0;
const int c_bankTypeFlag    = 1;
const int c_bankTypeInt     = 2;
const int c_bankTypeString  = 3;
const int c_bankTypeUnit    = 4;
const int c_bankTypePoint   = 5;
const int c_bankTypeText    = 6;

native bool     BankExists (string name, int player);
native bank     BankLastCreated ();
native bank     BankLoad (string name, int player);
native string   BankName (bank b);
native int      BankPlayer (bank b);
native void     BankRemove (bank b);
native void     BankSave (bank b);

native int      BankSectionCount (bank b);
native bool     BankSectionExists (bank b, string section);
native string   BankSectionName (bank b, int index);
native void     BankSectionRemove (bank b, string section);

native int      BankKeyCount (bank b, string section);
native bool     BankKeyExists (bank b, string section, string key);
native string   BankKeyName (bank b, string section, int index);
native void     BankKeyRemove (bank b, string section, string key);

native bool     BankValueIsType (bank b, string section, string key, int type);

native fixed    BankValueGetAsFixed (bank b, string section, string key);
native void     BankValueSetFromFixed (bank b, string section, string key, fixed value);

native bool     BankValueGetAsFlag (bank b, string section, string key);
native void     BankValueSetFromFlag (bank b, string section, string key, bool value);

native int      BankValueGetAsInt (bank b, string section, string key);
native void     BankValueSetFromInt (bank b, string section, string key, int value);

native point    BankValueGetAsPoint (bank b, string section, string key);
native void     BankValueSetFromPoint (bank b, string section, string key, point value);

native string   BankValueGetAsString (bank b, string section, string key);
native void     BankValueSetFromString (bank b, string section, string key, string value);

native text     BankValueGetAsText (bank b, string section, string key);
native void     BankValueSetFromText (bank b, string section, string key, text value);

native unit     BankLastRestoredUnit ();
native unit     BankValueGetAsUnit (bank b, string section, string key, int player, point p, fixed facing);
native void     BankValueSetFromUnit (bank b, string section, string key, unit value);

//--------------------------------------------------------------------------------------------------
// Battle Report
//--------------------------------------------------------------------------------------------------
const int c_invalidBattleReportId        = 0;

const int c_battleReportStateNormal     = 0;
const int c_battleReportStateCompleted  = 1;
const int c_battleReportStateHidden     = 2;

const int c_battleReportTypeMission     = 0;
const int c_battleReportTypeScene       = 1;

native void     BattleReportPanelSetSelectedBattleReport (playergroup players, int inBattleReport);
native int      BattleReportPanelGetSelectedBattleReport (int inPlayer);

native int      BattleReportCreate (playergroup inPlayerGroup, text inText, int inType, int inState);
native int      BattleReportLastCreated ();
native void     BattleReportDestroy (int inBattleReportId);

native void     BattleReportSetState (int inBattleReportId, int inState);
native void     BattleReportSetPriority (int inBattleReportId, int inPriority);
native void     BattleReportSetButtonText (int inBattleReportId, text inText);
native void     BattleReportSetButtonImage (int inBattleReportId, string inImage);

native void     BattleReportSetMissionText (int inBattleReportId, text inText);
native void     BattleReportSetResearchTitle (int inBattleReportId, text inText);
native void     BattleReportSetResearchText (int inBattleReportId, text inText);
native void     BattleReportSetBonusTitle (int inBattleReportId, text inText);
native void     BattleReportSetBonusText (int inBattleReportId, text inText);
native void     BattleReportSetWorldText (int inBattleReportId, text inText);
native void     BattleReportSetObjectiveTitle (int inBattleReportId, text inText);
native void     BattleReportSetObjectiveText (int inBattleReportId, text inText);
native void     BattleReportSetAchievementTitle (int inBattleReportId, text inText);
native void     BattleReportSetBestTimeText (int inBattleReportId, text inText);
native void     BattleReportSetMissionImage (int inBattleReportId, string inImage);
native void     BattleReportSetDifficultyLevelCompleted (int inBattleReportId, int inDifficultyLevel, bool inCompleted);
native void     BattleReportSetDifficultyLevelBestTimeText (int inBattleReportId, int inDifficultyLevel, text inText);
native void     BattleReportAddAchievement (int inBattleReportId, string inAchievement);

native void     BattleReportSetSceneText (int inBattleReportId, text inText);
native void     BattleReportSetSceneImage (int inBattleReportId, string inImage);

// Battle Report events
native void     TriggerAddEventBattleReportPanelExit (trigger t, int inPlayer);
native void     TriggerAddEventBattleReportPanelPlayMission (trigger t, int inPlayer);
native void     TriggerAddEventBattleReportPanelPlayScene (trigger t, int inPlayer);
native void     TriggerAddEventBattleReportPanelSelectionChanged (trigger t, int inPlayer);

native int      EventBattleReportPanelMissionSelected ();
native int      EventBattleReportPanelDifficultySelected ();
native int      EventBattleReportPanelSceneSelected ();

//--------------------------------------------------------------------------------------------------
// Boards
//--------------------------------------------------------------------------------------------------
const int c_boardNone = 0;

native int          BoardCreate (int inCols, int inRows, text inName, color inColor);
native int          BoardLastCreated ();
native void         BoardDestroy (int inBoard);

native void         BoardShowAll (bool inShow, playergroup inPlayers);

// Board title
const int c_boardColorText          = 0;
const int c_boardColorBackground    = 1;

const int c_boardIconPosLeft        = 0;
const int c_boardIconPosRight       = 1;

native void         BoardTitleShow (int inBoard, playergroup inPlayers, bool inShow);
native void         BoardTitleSetText (int inBoard, text inText);
native void         BoardTitleSetColor (int inBoard, int inType, color inColor);
native void         BoardTitleSetIcon (int inBoard, string inIcon);
native void         BoardTitleSetAlignment (int inBoard, int inAlign, int inIconPos);
native void         BoardTitleSetClickable (int inBoard, bool inClickable);

// Board properties
native void         BoardSetName (int inBoard, text inName, color inColor);

const int c_boardStateShowing       = 0;
const int c_boardStateShowTitle     = 1;
const int c_boardStateShowMinimize  = 2;
const int c_boardStateMinimized     = 3;
const int c_boardStateShowHeader    = 4;
const int c_boardStateSorted        = 5;

native void         BoardSetState (int inBoard, playergroup inPlayers, int inState, bool inVal);

// Board minimize button
native void         BoardMinimizeShow (int inBoard, playergroup inPlayers, bool inShow);
native void         BoardMinimizeEnable (int inBoard, playergroup inPlayers, bool inEnable);
native void         BoardMinimizeSetState (int inBoard, playergroup inPlayers, bool inVal);
native void         BoardMinimizeSetColor (int inBoard, color inColor);

// Board columns and rows
native void         BoardSetColumnCount (int inBoard, int inCols);
native void         BoardSetRowCount (int inBoard, int inRows);

const fixed c_boardWidthAuto    = 0; // Automatically size the column to fit all items

native void         BoardSetColumnWidth (int inBoard, int inCol, fixed inWidth);

// Board row groups
native void         BoardSetGroupCount (int inBoard, int inGroups);
native void         BoardRowSetGroup (int inBoard, int inRow, int inGroup);

// Board items
const int c_boardColGroups  = -3; // Use as column value to treat row value as group instead
const int c_boardItemAll    = -2; // Use as row or column value to modify all items
const int c_boardRowHeader  = -1; // Use as row value to access header items

native void         BoardItemSetText (int inBoard, int inCol, int inRow, text inText);
native void         BoardItemSetTextColor (int inBoard, int inCol, int inRow, color inColor);
native void         BoardItemSetBackgroundColor (int inBoard, int inCol, int inRow, color inColor);
native void         BoardItemSetIcon (int inBoard, int inCol, int inRow, string inIcon, bool inLeft);
native void         BoardItemSetAlignment (int inBoard, int inCol, int inRow, int inAlign);
native void         BoardItemSetFontSize (int inBoard, int inCol, int inRow, int inSize);
native void         BoardItemSetSortValue (int inBoard, int inCol, int inRow, int inVal);

native void         BoardItemSetProgressShow (int inBoard, int inCol, int inRow, bool inShow);
native void         BoardItemSetProgressRange (int inBoard, int inCol, int inRow, fixed inMin, fixed inMax);
native void         BoardItemSetProgressValue (int inBoard, int inCol, int inRow, fixed inVal);
native void         BoardItemSetProgressColor (int inBoard, int inCol, int inRow, color inColor, int inStep);

// Board sorting
// Note: inPriority defines multi-column sorting behavior.  If two items are identical, the next
//       sort priority is used (lowest priority value comes first).  inPriority should range
//       from 1 to the number of columns.
//
native void         BoardSort (int inBoard, int inCol, bool inAscending, int inPriority);

// Automatic player column
// Note: Once a player column has been added, the "row" values passed in to the functions above
//       should be the player id instead.
//
native void         BoardSetPlayerColumn (int inBoard, int inCol, bool inGroupByTeams);
native void         BoardPlayerAdd (int inBoard, int inPlayer);
native void         BoardPlayerRemove (int inBoard, int inPlayer);

//--------------------------------------------------------------------------------------------------
// Camera
//--------------------------------------------------------------------------------------------------
const int c_cameraValueFieldOfView      = 0;
const int c_cameraValueNearClip         = 1;
const int c_cameraValueFarClip          = 2;
const int c_cameraValueShadowClip       = 3;
const int c_cameraValueDistance         = 4;
const int c_cameraValuePitch            = 5;
const int c_cameraValueYaw              = 6;
const int c_cameraValueRoll             = 7;
const int c_cameraValueHeightOffset     = 8;
const int c_cameraValueDepthOfField     = 9;
const int c_cameraValueFocalDepth       = 10;
const int c_cameraValueFalloffStart     = 11;
const int c_cameraValueFalloffEnd       = 12;

const int c_cameraPositionEye           = 0;
const int c_cameraPositionTarget        = 1;
const int c_cameraPositionBoth          = 2;

const int c_cameraDirectionX            = 0;
const int c_cameraDirectionY            = 1;
const int c_cameraDirectionZ            = 2;
const int c_cameraDirectionXY           = 3;
const int c_cameraDirectionXZ           = 4;
const int c_cameraDirectionYZ           = 5;
const int c_cameraDirectionXYZ          = 6;

const int c_cameraRotationPitch         = 0;
const int c_cameraRotationRoll          = 1;
const int c_cameraRotationYaw           = 2;

// Camera Info
native camerainfo   CameraInfoDefault ();
native camerainfo   CameraInfoFromId (int id);

native void         CameraInfoSetValue (camerainfo c, int type, fixed value);
native fixed        CameraInfoGetValue (camerainfo c, int type);

native void         CameraInfoSetTarget (camerainfo c, point p);
native point        CameraInfoGetTarget (camerainfo c);

// Player Camera
native void         CameraApplyInfo (int player, camerainfo c, fixed duration, fixed velocity, fixed decelerate, bool useTarget);
native void         CameraPan (int player, point p, fixed duration, fixed velocity, fixed decelerate, bool smart);
native void         CameraSetValue (int player, int type, fixed value, fixed duration, fixed velocity, fixed decelerate);
native void         CameraUseModel (int player, unit u, string name, fixed duration);

native void         CameraForceMouseRelative (int player, bool value);
native void         CameraLockInput (int player, bool lock);
native void         CameraSetMouseRotates (int player, bool value);
native void         CameraSetMouseRotationSpeed (int player, int direction, fixed value);
native void         CameraSetVerticalFieldOfView (int player, bool value);
native void         CameraUseHeightDisplacement (int player, bool value);
native void         CameraUseHeightSmoothing (int player, bool value);

native void         CameraSetChannel (int player, unit cameraUnit, string cameraName, int channel, fixed aspectRatio);
native void         CameraClearChannel (int player, int channel);

native void         CameraSetChannelOnPortrait (int player, camerainfo c, fixed aspectRatio, int portraitId, int channel);
native void         CameraClearChannelOnPortrait (int player, int portraitId, int channel);

native void         CameraShakeStart (
                        int player,
                        int position,           // c_cameraPosition*
                        int direction,          // c_cameraDirection*
                        fixed amplitude,
                        fixed frequency,
                        fixed randomPercent,
                        fixed duration
                    );
native void         CameraShakeStop (int player);

// - CameraSave saves the current camera settings for the given player,
//   which can later be restored using CameraRestore.
//
native void         CameraSave (int player);
native void         CameraRestore (int player, fixed duration, fixed velocity, fixed decelerate);

// - CameraGetTarget returns the synchronized target for the given player
//
native point        CameraGetTarget (int player);
native fixed        CameraGetPitch (int player);
native fixed        CameraGetYaw (int player);

// Camera Bounds, optionally adjusting the minimap as well
// Camera bounds can never extend beyond the playable map area
//
native void         CameraSetBounds (playergroup players, region bounds, bool includeMinimap);

// Force the camera to start/stop following a group of units
//
native void         CameraFollowUnitGroup (int player, unitgroup group, bool follow, bool isOffset);

native void         CameraLookAt (int player, point p, fixed duration, fixed velocity, fixed decelerate);
// Make the camera look at an actor or unit, updating as it moves. Use null to stop looking.
//
native void         CameraLookAtActor (int player, actor a);
native void         CameraLookAtUnit (int player, unit u);

// Camera movement events
const int c_cameraMoveReasonAny             = -1;
const int c_cameraMoveReasonAlert           = 0;
const int c_cameraMoveReasonKeyScroll       = 1;
const int c_cameraMoveReasonMinimap         = 2;
const int c_cameraMoveReasonMouseScroll     = 3;
const int c_cameraMoveReasonSelection       = 4;
const int c_cameraMoveReasonTown            = 5;
const int c_cameraMoveReasonView            = 6;

native void         TriggerAddEventCameraMove (trigger t, int player, int reason);
native int          EventCameraMoveReason ();

//--------------------------------------------------------------------------------------------------
// Campaign
//
// Game initialization and configuration associated with standard campaign games
//--------------------------------------------------------------------------------------------------
native void     CampaignInitAI ();
native void        CampaignProgressSetText (playergroup players, string campaignId, text inText); // Blizzard maps only
native void        CampaignProgressSetImageFilePath (playergroup players, string campaignId, string inFilePath); // Blizzard maps only
native void        CampaignProgressSetTutorialFinished (playergroup players, string campaignId, bool inFinished); // Blizzard maps only
native void        CampaignProgressSetCampaignFinished (playergroup players, string campaignId, bool inFinished); // Blizzard maps only

//--------------------------------------------------------------------------------------------------
// Catalogs
//
// Note: The catalog entry table includes references to entries from other fields,
//       even if that entry is not actually defined.  This means it is possible that
//       enumerating entries via CatalogEntryCount and CatalogEntryGet may return entries
//       that don't actually exist.  Use CatalogEntryIsValid to check for this case.
//
//--------------------------------------------------------------------------------------------------
native int AbilityClass (string ability);

native int CatalogEntryCount (int catalog);
native string CatalogEntryGet (int catalog, int index);

native bool CatalogEntryIsValid (int catalog, string entry);
native int CatalogEntryClass (int catalog, string entry);
native string CatalogEntryParent (int catalog, string entry);
native string CatalogEntryScope (int catalog, string entry);

native int CatalogFieldCount (string scope);
native string CatalogFieldGet (string scope, int index);
native bool CatalogFieldIsArray (string scope, string field);
native bool CatalogFieldIsScope (string scope, string field);
native string CatalogFieldType (string scope, string field);

native int CatalogFieldValueCount (int catalog, string entry, string fieldPath, int player);
native string CatalogFieldValueGet (int catalog, string entry, string fieldPath, int player);
native bool CatalogFieldValueSet (int catalog, string entry, string fieldPath, int player, string value);

//--------------------------------------------------------------------------------------------------
// CharacterSheet
//--------------------------------------------------------------------------------------------------
native void     CharacterSheetPanelSetNameText (playergroup players, text inText);
native void     CharacterSheetPanelSetDescriptionText (playergroup players, text inText);
native void     CharacterSheetPanelSetPortraitModelLink (playergroup players, string inModelLink);

//--------------------------------------------------------------------------------------------------
// Cinematics
//--------------------------------------------------------------------------------------------------
native void CinematicMode (playergroup players, bool cinematicMode, fixed duration);

const int c_fadeStyleNormal            = 0;
const int c_fadeStyleExponential    = 1;
const int c_fadeStyleSine            = 2;
const int c_fadeStyleSquareRoot        = 3;

native void CinematicFade (
    bool fadeIn,
    fixed duration,
    int style,
    color inColor,
    fixed transparency,
    bool waitUntilDone
);

native void CinematicOverlay (
    bool fadeIn,
    fixed duration,
    string imagePath,
    fixed transparency,
    bool waitUntilDone
);

native void CinematicDataRun (int id, playergroup players, bool waitUntilDone);
native void CinematicDataStop ();

//--------------------------------------------------------------------------------------------------
// Conversions
//--------------------------------------------------------------------------------------------------
native int      BoolToInt (bool f);
native int      Color255FromFixed (fixed f);

native fixed    IntToFixed (int x);
native string   IntToString (int x);
native text     IntToText (int x);

const int c_fixedPrecisionAny = -1;
native int      FixedToInt (fixed x);
native string   FixedToString (fixed x, int precision);
native text     FixedToText (fixed x, int precision);

native int      StringToInt (string x);
native fixed    StringToFixed (string x);

// Color
// Note: Component values are in percent (0-100)
//
const int c_colorComponentRed   = 0;
const int c_colorComponentGreen = 1;
const int c_colorComponentBlue  = 2;
const int c_colorComponentAlpha = 3;

native color    Color (fixed r, fixed g, fixed b);
native color    ColorWithAlpha (fixed r, fixed g, fixed b, fixed a);
native color    ColorFromIndex (int inIndex, int inType);
native fixed    ColorGetComponent (color c, int component);

native text     FormatNumber (int number);
native text     FormatDuration (int seconds);

//--------------------------------------------------------------------------------------------------
// Conversations
//--------------------------------------------------------------------------------------------------
const int c_invalidConversationId           = 0;
const int c_invalidReplyId                  = 0;

const int c_conversationReplyStateUnread    = 0;
const int c_conversationReplyStateRead      = 1;
const int c_conversationReplyStateOld       = 2;

native int          ConversationCreate (bool visible);
native int          ConversationLastCreated ();
native void         ConversationDestroy (int intId);
native void         ConversationDestroyAll ();

native void         ConversationShow (int intId, playergroup to, bool visible);
native bool         ConversationVisible (int intId, int player);

native int          ConversationReplyCreate (int intId, text inText);
native int          ConversationReplyLastCreated ();
native void         ConversationReplyDestroy (int intId, int replyId);
native void         ConversationReplyDestroyAll (int intId);

native void         ConversationReplySetText (int intId, int replyId, text inText);
native text         ConversationReplyGetText (int intId, int replyId);

native void         ConversationReplySetState (int intId, int replyId, int state);
native int          ConversationReplyGetState (int intId, int replyId);

// Conversation events
native void         TriggerAddEventConversationReplySelected (trigger t, int player, int intId, int replyId);

native int          EventConversation ();
native int          EventConversationReply ();

native int          ConversationReplyGetIndex (int intId, int replyId);

// Data-driven conversations
// Note: The stateIndex parameters specify the state id and the index id (if any), separated by "|".
//       ConversationDataStateIndex may be used to assemble the proper string for a given numerical
//       index in the state definition.
//
native int          ConversationDataStateIndexCount (string inStateId);
native string       ConversationDataStateIndex (string inStateId, int inIndex);

native text         ConversationDataStateName (string stateIndex);
native text         ConversationDataStateText (string stateIndex, string inInfoName);
native fixed        ConversationDataStateFixedValue (string stateIndex, string inInfoName);
native string       ConversationDataStateImagePath (string stateIndex);
native int          ConversationDataStateImageEdge (string stateIndex);
native string       ConversationDataStateAttachPoint (string stateIndex);
native string       ConversationDataStateMoviePath (string stateIndex);
native string       ConversationDataStateModel (string stateIndex, string inInfoName);
native string       ConversationDataStateUpgrade (string stateIndex, string inInfoName);
native abilcmd      ConversationDataStateAbilCmd (string stateIndex, string inInfoName);

native void         ConversationDataRegisterCamera (string camIndex, string charIndex, camerainfo c, trigger t, bool wait);
native void         ConversationDataRegisterUnit (string stateIndex, unit u);
native void         ConversationDataRegisterPortrait (string stateIndex, int p);

native void         ConversationDataStateSetValue (string stateIndex, int value);
native int          ConversationDataStateGetValue (string stateIndex);

native int          ConversationDataChoiceCount (string convId);
native string       ConversationDataChoiceId (string convId, int index);
native void         ConversationDataChoiceSetState (string convId, string choiceId, int state);
native int          ConversationDataChoiceGetState (string convId, string choiceId);
native void         ConversationDataChoiceSetPicked (string convId, string choiceId, bool picked);
native bool         ConversationDataChoiceGetPicked (string convId, string choiceId);
native void         ConversationDataChoiceSetPickedCount (string convId, string choiceId, int count);
native int          ConversationDataChoiceGetPickedCount (string convId, string choiceId);

native int          ConversationDataLineCount (string convId);
native string       ConversationDataLineId (string convId, int index);
native void         ConversationDataLineSetPickedCount (string convId, string lineId, int count);
native int          ConversationDataLineGetPickedCount (string convId, string lineId);

// State save/load via bank
// - State values are the arbitrary integer values associated with state objects.
//
native void         ConversationDataSaveStateValues (string stateId, bank b, string section);
native void         ConversationDataLoadStateValues (string stateId, bank b, string section);

native void         ConversationDataResetStateValues (string stateId);

// - Node state is the read/unread and picked state associated with lines and choices (nodes)
//   within a given conversation.
//
native void         ConversationDataSaveNodeState (string convId, bank b, string section);
native void         ConversationDataLoadNodeState (string convId, bank b, string section);

native void         ConversationDataResetNodeState (string convId);

// ConversationDataPreloadLines preloads sounds for all lines that are available to run given
// the current state.
//
native void         ConversationDataPreloadLines (string convId);

// ConversationDataCanRun returns true if the given conversation has any lines or choices available
// to run given the current state.
//
native bool         ConversationDataCanRun (string convId, bool unpickedOnly);

const int c_conversationSkipNone    = 0;    // Skipping not allowed
const int c_conversationSkipSimple  = 1;    // Skipping only allowed for all lines (esc key),
                                            // not individual lines (space bar, left click)
const int c_conversationSkipFull    = 2;    // Skipping allowed for all lines or individual lines

native void         ConversationDataRun (string convId, playergroup players, int skipType, bool waitUntilDone);
native void         ConversationDataStop ();

native string       ConversationDataActiveSound ();     // Sound id associated with the active line
native string       ConversationDataActiveCamera ();    // Last camera state to be applied

// Data Conversation events
native void         TriggerAddEventConversationStateChanged (trigger t, string stateIndex);

native string       EventConversationState ();

//--------------------------------------------------------------------------------------------------
// Data Table
// - Data tables provide named storage for any script type.
//   Table access may be either global or thread-local.
//--------------------------------------------------------------------------------------------------
// Types
const int c_dataTypeUnknown             = -1;
const int c_dataTypeAbilCmd             =  0;
const int c_dataTypeActor               =  1;
const int c_dataTypeActorScope          =  2;
const int c_dataTypeAIFilter            =  3;
const int c_dataTypeBank                =  4;
const int c_dataTypeBool                =  5;
const int c_dataTypeByte                =  6;
const int c_dataTypeCameraInfo          =  7;
const int c_dataTypeCinematic           =  8;
const int c_dataTypeColor               =  9;
const int c_dataTypeControl             = 10;
const int c_dataTypeConversation        = 11;
const int c_dataTypeDialog              = 12;
const int c_dataTypeDoodad              = 13;
const int c_dataTypeFixed               = 14;
const int c_dataTypeInt                 = 15;
const int c_dataTypeMarker              = 16;
const int c_dataTypeObjective           = 17;
const int c_dataTypeOrder               = 18;
const int c_dataTypePing                = 19;
const int c_dataTypePlanet              = 20;
const int c_dataTypePlayerGroup         = 21;
const int c_dataTypePoint               = 22;
const int c_dataTypePortrait            = 23;
const int c_dataTypeRegion              = 24;
const int c_dataTypeReply               = 25;
const int c_dataTypeRevealer            = 26;
const int c_dataTypeRoom                = 27;
const int c_dataTypeSound               = 28;
const int c_dataTypeSoundLink           = 29;
const int c_dataTypeString              = 30;
const int c_dataTypeText                = 31;
const int c_dataTypeTimer               = 32;
const int c_dataTypeTransmission        = 33;
const int c_dataTypeTransmissionSource  = 34;
const int c_dataTypeTrigger             = 35;
const int c_dataTypeUnit                = 36;
const int c_dataTypeUnitFilter          = 37;
const int c_dataTypeUnitGroup           = 38;
const int c_dataTypeUnitRef             = 39;
const int c_dataTypeWave                = 40;
const int c_dataTypeWaveInfo            = 41;
const int c_dataTypeWaveTarget          = 42;

// General functionality
native void     DataTableClear (bool global);
native int      DataTableValueCount (bool global);
native string   DataTableValueName (bool global, int index);
native bool     DataTableValueExists (bool global, string name);
native int      DataTableValueType (bool global, string name);
native void     DataTableValueRemove (bool global, string name);

// Type-specific value set/get
// - c_dataTypeAbilCmd
native void         DataTableSetAbilCmd (bool global, string name, abilcmd val);
native abilcmd      DataTableGetAbilCmd (bool global, string name);

// - c_dataTypeActor
native void         DataTableSetActor (bool global, string name, actor val);
native actor        DataTableGetActor (bool global, string name);

// - c_dataTypeActorScope
native void         DataTableSetActorScope (bool global, string name, actorscope val);
native actorscope   DataTableGetActorScope (bool global, string name);

// - c_dataTypeAIFilter
native void         DataTableSetAIFilter (bool global, string name, aifilter val);
native aifilter     DataTableGetAIFilter (bool global, string name);

// - c_dataTypeBank
native void         DataTableSetBank (bool global, string name, bank val);
native bank         DataTableGetBank (bool global, string name);

// - c_dataTypeBool
native void         DataTableSetBool (bool global, string name, bool val);
native bool         DataTableGetBool (bool global, string name);

// - c_dataTypeByte
native void         DataTableSetByte (bool global, string name, byte val);
native byte         DataTableGetByte (bool global, string name);

// - c_dataTypeCameraInfo
native void         DataTableSetCameraInfo (bool global, string name, camerainfo val);
native camerainfo   DataTableGetCameraInfo (bool global, string name);

// - c_dataTypeCinematic
native void         DataTableSetCinematic (bool global, string name, int val);
native int          DataTableGetCinematic (bool global, string name);

// - c_dataTypeColor
native void         DataTableSetColor (bool global, string name, color val);
native color        DataTableGetColor (bool global, string name);

// - c_dataTypeControl
native void         DataTableSetControl (bool global, string name, int val);
native int          DataTableGetControl (bool global, string name);

// - c_dataTypeConversation
native void         DataTableSetConversation (bool global, string name, int val);
native int          DataTableGetConversation (bool global, string name);

// - c_dataTypeDialog
native void         DataTableSetDialog (bool global, string name, int val);
native int          DataTableGetDialog (bool global, string name);

// - c_dataTypeDoodad
native void         DataTableSetDoodad (bool global, string name, doodad val);
native doodad       DataTableGetDoodad (bool global, string name);

// - c_dataTypeFixed
native void         DataTableSetFixed (bool global, string name, fixed val);
native fixed        DataTableGetFixed (bool global, string name);

// - c_dataTypeInt
native void         DataTableSetInt (bool global, string name, int val);
native int          DataTableGetInt (bool global, string name);

// - c_dataTypeMarker
native void         DataTableSetMarker (bool global, string name, marker val);
native marker       DataTableGetMarker (bool global, string name);

// - c_dataTypeObjective
native void         DataTableSetObjective (bool global, string name, int val);
native int          DataTableGetObjective (bool global, string name);

// - c_dataTypeOrder
native void         DataTableSetOrder (bool global, string name, order val);
native order        DataTableGetOrder (bool global, string name);

// - c_dataTypePing
native void         DataTableSetPing (bool global, string name, int val);
native int          DataTableGetPing (bool global, string name);

// - c_dataTypePlanet
native void         DataTableSetPlanet (bool global, string name, int val);
native int          DataTableGetPlanet (bool global, string name);

// - c_dataTypePlayerGroup
native void         DataTableSetPlayerGroup (bool global, string name, playergroup val);
native playergroup  DataTableGetPlayerGroup (bool global, string name);

// - c_dataTypePoint
native void         DataTableSetPoint (bool global, string name, point val);
native point        DataTableGetPoint (bool global, string name);

// - c_dataTypePortrait
native void         DataTableSetPortrait (bool global, string name, int val);
native int          DataTableGetPortrait (bool global, string name);

// - c_dataTypeRegion
native void         DataTableSetRegion (bool global, string name, region val);
native region       DataTableGetRegion (bool global, string name);

// - c_dataTypeReply
native void         DataTableSetReply (bool global, string name, int val);
native int          DataTableGetReply (bool global, string name);

// - c_dataTypeRevealer
native void         DataTableSetRevealer (bool global, string name, revealer val);
native revealer     DataTableGetRevealer (bool global, string name);

// - c_dataTypeRoom
native void         DataTableSetRoom (bool global, string name, int val);
native int          DataTableGetRoom (bool global, string name);

// - c_dataTypeSound
native void         DataTableSetSound (bool global, string name, sound val);
native sound        DataTableGetSound (bool global, string name);

// - c_dataTypeSoundLink
native void         DataTableSetSoundLink (bool global, string name, soundlink val);
native soundlink    DataTableGetSoundLink (bool global, string name);

// - c_dataTypeString
native void         DataTableSetString (bool global, string name, string val);
native string       DataTableGetString (bool global, string name);

// - c_dataTypeText
native void         DataTableSetText (bool global, string name, text val);
native text         DataTableGetText (bool global, string name);

// - c_dataTypeTimer
native void         DataTableSetTimer (bool global, string name, timer val);
native timer        DataTableGetTimer (bool global, string name);

// - c_dataTypeTransmission
native void         DataTableSetTransmission (bool global, string name, int val);
native int          DataTableGetTransmission (bool global, string name);

// - c_dataTypeTransmissionSource
native void                 DataTableSetTransmissionSource (bool global, string name, transmissionsource val);
native transmissionsource   DataTableGetTransmissionSource (bool global, string name);

// - c_dataTypeTrigger
native void         DataTableSetTrigger (bool global, string name, trigger val);
native trigger      DataTableGetTrigger (bool global, string name);

// - c_dataTypeUnit
native void         DataTableSetUnit (bool global, string name, unit val);
native unit         DataTableGetUnit (bool global, string name);

// - c_dataTypeUnitFilter
native void         DataTableSetUnitFilter (bool global, string name, unitfilter val);
native unitfilter   DataTableGetUnitFilter (bool global, string name);

// - c_dataTypeUnitGroup
native void         DataTableSetUnitGroup (bool global, string name, unitgroup val);
native unitgroup    DataTableGetUnitGroup (bool global, string name);

// - c_dataTypeUnitRef
native void         DataTableSetUnitRef (bool global, string name, unitref val);
native unitref      DataTableGetUnitRef (bool global, string name);

// - c_dataTypeWave
native void         DataTableSetWave (bool global, string name, wave val);
native wave         DataTableGetWave (bool global, string name);

// - c_dataTypeWaveInfo
native void         DataTableSetWaveInfo (bool global, string name, waveinfo val);
native waveinfo     DataTableGetWaveInfo (bool global, string name);

// - c_dataTypeWaveTarget
native void         DataTableSetWaveTarget (bool global, string name, wavetarget val);
native wavetarget   DataTableGetWaveTarget (bool global, string name);

//--------------------------------------------------------------------------------------------------
// Dialogs
//--------------------------------------------------------------------------------------------------
const int c_invalidDialogId     = 0;

native int      DialogCreate (
                    int width,
                    int height,
                    int anchor,
                    int offsetX,
                    int offsetY,
                    bool modal
                );
native int      DialogLastCreated ();
native void     DialogDestroy (int dialog);
native void     DialogDestroyAll ();

native void        DialogSetSubtitlePositionOverride (int dialog);
native void        DialogClearSubtitlePositionOverride ();

native void     DialogSetTitle (int dialog, text title);
native void     DialogSetSize (int dialog, int width, int height);
native void     DialogSetPosition (int dialog, int anchor, int offsetX, int offsetY);
native void     DialogSetPositionRelative (int dialog, int anchor, int relative, int relativeAnchor, int offsetX, int offsetY);
native void     DialogSetVisible (int dialog, playergroup players, bool isVisible);
native void     DialogSetTransparency (int dialog, fixed inTransparency);
native void     DialogSetImage (int dialog, string image);
native void     DialogSetImageVisible (int dialog, bool isVisible);
native void     DialogSetOffscreen (int dialog, bool isOffscreen);
native void     DialogSetFullscreen (int dialog, bool isFullscreen);
native void     DialogSetChannel (int dialog, int channel);

native bool     DialogIsModal (int dialog);
native text     DialogGetTitle (int dialog);
native int      DialogGetWidth (int dialog);
native int      DialogGetHeight (int dialog);
native int      DialogGetAnchor (int dialog);
native int      DialogGetRelativeDialog (int dialog);
native int      DialogGetRelativeAnchor (int dialog);
native int      DialogGetOffsetX (int dialog);
native int      DialogGetOffsetY (int dialog);
native bool     DialogIsVisible (int dialog, int player);
native fixed    DialogGetTransparency (int dialog);
native string   DialogGetImage (int dialog);
native bool     DialogIsImageVisible (int dialog);
native bool     DialogIsOffscreen (int dialog);
native bool     DialogIsFullscreen (int dialog);
native int      DialogGetChannel (int dialog);

// Control types
const int c_triggerControlTypeInvalid                   = 0;
const int c_triggerControlTypeLabel                     = 1;
const int c_triggerControlTypeImage                     = 2;
const int c_triggerControlTypeButton                    = 3;
const int c_triggerControlTypeCheckBox                  = 4;
const int c_triggerControlTypeListBox                   = 5;
const int c_triggerControlTypePulldown                  = 6;
const int c_triggerControlTypeProgressBar               = 7;
const int c_triggerControlTypeSlider                    = 8;
const int c_triggerControlTypeEditBox                   = 9;
const int c_triggerControlTypeAchievement               = 11;

// Control properties
const int c_triggerControlPropertyInvalid               = 0;
const int c_triggerControlPropertyText                  = 1;        // text
const int c_triggerControlPropertyStyle                 = 3;        // string
const int c_triggerControlPropertyImage                 = 4;        // string
const int c_triggerControlPropertyImageType             = 5;        // int
const int c_triggerControlPropertyColor                 = 6;        // color
const int c_triggerControlPropertyChecked               = 7;        // bool
const int c_triggerControlPropertyMinValue              = 8;        // fixed
const int c_triggerControlPropertyMaxValue              = 9;        // fixed
const int c_triggerControlPropertyValue                 = 10;       // fixed
const int c_triggerControlPropertyTooltip               = 11;       // text
const int c_triggerControlPropertyVisible               = 12;       // bool
const int c_triggerControlPropertyEnabled               = 13;       // bool
const int c_triggerControlPropertyWidth                 = 14;       // int
const int c_triggerControlPropertyHeight                = 15;       // int
const int c_triggerControlPropertyAnchor                = 16;       // int
const int c_triggerControlPropertyRelative              = 17;       // int
const int c_triggerControlPropertyRelativeAnchor        = 18;       // int
const int c_triggerControlPropertyOffsetX               = 19;       // int
const int c_triggerControlPropertyOffsetY               = 20;       // int
const int c_triggerControlPropertyEditText              = 21;       // string
const int c_triggerControlPropertyItemCount             = 22;       // int
const int c_triggerControlPropertySelectionIndex        = 23;       // int
const int c_triggerControlPropertyFile                  = 24;       // string
const int c_triggerControlPropertyOffscreen             = 25;       // bool
const int c_triggerControlPropertyChannel               = 26;       // int
const int c_triggerControlPropertyFullDialog            = 27;       // bool
const int c_triggerControlPropertyTextWriteout          = 28;       // bool
const int c_triggerControlPropertyTextWriteoutDuration  = 29;       // fixed
const int c_triggerControlPropertyBlendMode                = 30;       // int
const int c_triggerControlPropertyHoverImage            = 31;       // string
const int c_triggerControlPropertyTiled                    = 32;       // bool
const int c_triggerControlPropertyRotation              = 33;       // int
const int c_triggerControlPropertyAchievement           = 34;       // string
const int c_triggerControlPropertyRenderPriority        = 35;       // int

// Image types
const int c_triggerImageTypeNone                        = 0;
const int c_triggerImageTypeNormal                      = 1;
const int c_triggerImageTypeBorder                      = 2;
const int c_triggerImageTypeHorizontalBorder            = 3;
const int c_triggerImageTypeEndCap                      = 4;

// Blend Modes
const int c_triggerBlendModeNormal            = 0;
const int c_triggerBlendModeMultiply          = 1;
const int c_triggerBlendModeLighten           = 2;
const int c_triggerBlendModeDarken            = 3;
const int c_triggerBlendModeAdd               = 4;
const int c_triggerBlendModeSubtract          = 5;
const int c_triggerBlendModeAlpha             = 6;

// Item constants
const int c_invalidDialogControlId                      = 0;

const int c_dialogControlItemNone                       = -1;

// Controls
native int      DialogControlCreate (int dialog, int type);
native int      DialogControlCreateFromTemplate (int dialog, int type, string inTemplate);
native int      DialogControlLastCreated ();
native void     DialogControlDestroy (int control);
native void     DialogControlDestroyAll (int dialog);

native void     DialogControlSetSize (int control, playergroup players, int width, int height);
native void     DialogControlSetPosition (int control, playergroup players, int anchor, int offsetX, int offsetY);
native void     DialogControlSetPositionRelative (int control, playergroup players, int anchor, int relative, int relativeAnchor, int offsetX, int offsetY);
native void     DialogControlSetVisible (int control, playergroup players, bool isVisible);
native void     DialogControlSetEnabled (int control, playergroup players, bool isEnabled);
native void     DialogControlSetFullDialog (int control, playergroup players, bool isFullDialog);
native void     DialogControlFadeTransparency  (int control, playergroup players, fixed fadeTime, fixed inTargetTransparency);

native int      DialogControlGetType (int control);
native int      DialogControlGetWidth (int control, int player);
native int      DialogControlGetHeight (int control, int player);
native int      DialogControlGetAnchor (int control, int player);
native int      DialogControlGetRelativeControl (int control, int player);
native int      DialogControlGetRelativeAnchor (int control, int player);
native int      DialogControlGetOffsetX (int control, int player);
native int      DialogControlGetOffsetY (int control, int player);
native bool     DialogControlIsVisible (int control, int player);
native bool     DialogControlIsEnabled (int control, int player);
native bool     DialogControlIsFullDialog (int control, int player);

native void     DialogControlSetPropertyAsText (int control, int property, playergroup players, text value);
native void     DialogControlSetPropertyAsString (int control, int property, playergroup players, string value);
native void     DialogControlSetPropertyAsInt (int control, int property, playergroup players, int value);
native void     DialogControlSetPropertyAsFixed (int control, int property, playergroup players, fixed value);
native void     DialogControlSetPropertyAsBool (int control, int property, playergroup players, bool value);
native void     DialogControlSetPropertyAsColor (int control, int property, playergroup players, color value);
native void     DialogControlSetPropertyAsControl (int control, int property, playergroup players, int value);

native text     DialogControlGetPropertyAsText (int control, int property, int player);
native string   DialogControlGetPropertyAsString (int control, int property, int player);
native int      DialogControlGetPropertyAsInt (int control, int property, int player);
native fixed    DialogControlGetPropertyAsFixed (int control, int property, int player);
native bool     DialogControlGetPropertyAsBool (int control, int property, int player);
native color    DialogControlGetPropertyAsColor (int control, int property, int player);
native int      DialogControlGetPropertyAsControl (int control, int property, int player);

native void     DialogControlAddItem (int control, playergroup players, text value);
native void     DialogControlRemoveItem (int control, playergroup players, int index);
native void     DialogControlRemoveAllItems (int control, playergroup players);
native void     DialogControlSelectItem (int control, playergroup players, int index);
native int      DialogControlGetItemCount (int control, int player);
native int      DialogControlGetSelectedItem (int control, int player);

native void     DialogControlInvokeAsText (int control, playergroup players, string method, text param1, text param2, text param3, text param4); // Blizzard maps only
native void     DialogControlInvokeAsString (int control, playergroup players, string method, string param1, string param2, string param3, string param4); // Blizzard maps only

// Dialog events
const int c_dialogControlAny = -1;

const int c_triggerControlEventTypeAny                       = -1;
const int c_triggerControlEventTypeClick                     = 0;
const int c_triggerControlEventTypeChecked                   = 1;
const int c_triggerControlEventTypeValueChanged              = 2;
const int c_triggerControlEventTypeSelectionChanged          = 3;
const int c_triggerControlEventTypeSelectionDoubleClicked    = 4;
const int c_triggerControlEventTypeTextChanged               = 5;

native void     TriggerAddEventDialogControl (trigger t, int player, int control, int eventType);

native int      EventDialogControl ();
native int      EventDialogControlEventType ();

//--------------------------------------------------------------------------------------------------
// Environment
//--------------------------------------------------------------------------------------------------
// Time Of Day
native string   GameTimeOfDayGet ();
native void     GameTimeOfDaySet (string x);
native fixed    GameTimeOfDayGetLength ();  // Length of a day in game time seconds
native void     GameTimeOfDaySetLength (fixed inSecs);
native void     GameTimeOfDayPause (bool inPause);
native bool     GameTimeOfDayIsPaused ();

// Creep
native int      CreepAdjacent (point inPos);
native bool     CreepIsPresent (point inPos);
native void     CreepModify (point inPos, fixed inRadius, bool inAdd, bool inPermanent);

const int c_creepSpeedGrowth    = 0;
const int c_creepSpeedDecay     = 1;
const int c_creepSpeedBlend     = 2;

native void     CreepSetSpeed (int inType, fixed inPercent);

// Pathing
// - Modifications made using PathingModify will only take effect after:
//   a) PathingUpdate is called
//   - OR -
//   b) The end of the game loop
//
// - PathingReset resets *all* trigger-based modifications
//
const int c_pathingNoPathing    = 0;
const int c_pathingNoBuilding   = 1;
const int c_pathingGround       = 2;

native void     PathingModify (region inArea, int inType, bool inAdd);
native void     PathingUpdate ();
native void     PathingReset ();

// Power
native int      PowerLevel (int inPlayer, point inPos);
native bool     PowerIsProvidedBy (int inPlayer, point inPos, unit inSource, int inMinLevel);

// Height
native int      CliffLevel (point inPos);
native fixed    WorldHeight (int inType, point inPos);  // inType uses the c_heightMap<*> values

// Miscellaneous
const int c_backgroundFixed     = 0; // Attached to camera position (never appears to move)
const int c_backgroundTerrain   = 1; // Attached to terrain (moves as camera scrolls)

native void     GameSetLighting (string inId, fixed inBlendTime);
native void     GameSetToDLighting (string inId);
native void     SelectMainShadowLight (string inId);
native void     GameSetBackground (int inType, string inModel, fixed inAnimSpeed);
native void     GameDestroyEffects (point pos, fixed radius, int maxCount, string effectType);

native void     TerrainShowRegion (region inArea, bool inShow);

const int c_wtcLinear       = 0;
const int c_wtcSine         = 1;
const int c_wtcExponential  = 2;
const int c_wtcSquareRoot   = 3;

// First parameter is Water Template + Water State in one string, delimited by |
native void     WaterSetState (string inWater, fixed inDuration, int inMorphType);

// Fog
native void     FogSetEnabled (bool f);
native void     FogSetDensity (fixed d);
native void     FogSetColor (color c);
native void     FogSetFallOff (fixed f);
native void     FogSetStartHeight (fixed h);

// General environment visibility
const int c_environmentAll                  = 0;    // All of the below
const int c_environmentTerrain              = 1;
const int c_environmentWater                = 2;
const int c_environmentDoodads              = 3;
const int c_environmentBackgroundFixed      = 4;
const int c_environmentBackgroundTerrain    = 5;

native void     EnvironmentShow (int inType, bool inShow);

//--------------------------------------------------------------------------------------------------
// Game
//--------------------------------------------------------------------------------------------------
native bool     ConsoleCommand (string inText, bool allowDefault, bool allowMacros);
native bool        GameIsDebugOptionSet (string inOptionName, int player);

const int c_gameSpeedSlower     = 0;
const int c_gameSpeedSlow       = 1;
const int c_gameSpeedNormal     = 2;
const int c_gameSpeedFast       = 3;
const int c_gameSpeedFaster     = 4;

native text     GameMapName ();
native text     GameMapDescription ();

native fixed    GameGetSpeed ();
native void     GameSetSpeedValue (int speed);
native int      GameGetSpeedValue ();
native void     GameSetSpeedValueMinimum (int speed);
native int      GameGetSpeedValueMinimum ();
native void     GameSetSpeedLocked (bool isLocked);
native bool     GameIsSpeedLocked ();

native string   GameAttributeGameValue (string name);
native string   GameAttributePlayerValue (string name, int player);

native void     GameSetSeedLocked (bool locked);
native bool     GameIsSeedLocked ();

native void     GameSetAbsoluteTimeRemaining (fixed inTime);
native fixed    GameGetAbsoluteTimeRemaining ();
native void     GameSetAbsoluteTimeRemainingPaused (bool inPaused);
native bool     GameGetAbsoluteTimeRemainingPaused ();

native void     GamePauseAllCharges (bool inPaused);
native void     GamePauseAllCooldowns (bool inPaused);

native void     GameAddChargeRegen (string inCharge, fixed inVal);
native fixed    GameGetChargeRegen (string inCharge);
native void     GameAddChargeUsed (string inCharge, fixed inVal);
native fixed    GameGetChargeUsed (string inCharge);

native void     GameAddCooldown (string inCooldown, fixed inVal);
native fixed    GameGetCooldown (string inCooldown);

native bool     GameIsTestMap (bool inAuto);

native void     GameSetNextMap (string inMap);
native void     SetNextMissionDifficulty (playergroup inPlayerGroup, int inDifficultyLevel);
native bool     GameIsTransitionMap ();
native void     GameSetTransitionMap (string transitionMap);

native string   GameTerrainSet ();
native void     GameWaitForResourcesToComplete ();

const int c_gameOverVictory = 0;
const int c_gameOverDefeat  = 1;
const int c_gameOverTie     = 2;

native void     GameOver (int player, int type, bool showDialog, bool showScore);
native void        RestartGame (playergroup inPlayerGroup);

// Game Events
native void     TriggerAddEventMapInit (trigger t);
native void     TriggerAddEventSaveGame (trigger t);
native void     TriggerAddEventChatMessage (trigger t, int player, string inText, bool exact);

native string   EventChatMessage (bool matchedOnly);

//--------------------------------------------------------------------------------------------------
// Looping
//
// Convenience functions to allow for loops without creating any local variables.
// Notes:
// - The loop data is stored locally to the executing thread
// - Nested loops (of the same type) are not supported, and will produce a run-time error
//
//--------------------------------------------------------------------------------------------------
// Integer
// - Start and end are inclusive
//
native void IntLoopBegin (int start, int end);
native void IntLoopStep ();
native bool IntLoopDone ();
native int  IntLoopCurrent ();
native void IntLoopEnd ();

// Player group
native void PlayerGroupLoopBegin (playergroup g);
native void PlayerGroupLoopStep ();
native bool PlayerGroupLoopDone ();
native int  PlayerGroupLoopCurrent ();
native void PlayerGroupLoopEnd ();

// Unit group
native void UnitGroupLoopBegin (unitgroup g);
native void UnitGroupLoopStep ();
native bool UnitGroupLoopDone ();
native unit UnitGroupLoopCurrent ();
native void UnitGroupLoopEnd ();

//--------------------------------------------------------------------------------------------------
// Markers
//--------------------------------------------------------------------------------------------------
native marker   Marker (string link);
native marker   MarkerCastingPlayer (string link, int player);
native marker   MarkerCastingUnit (string link, unit u);

native void     MarkerSetCastingPlayer (marker m, int player);
native int      MarkerGetCastingPlayer (marker m);

native void     MarkerSetCastingUnit (marker m, unit u);
native unit     MarkerGetCastingUnit (marker m);

native void     MarkerSetMatchFlag (marker m, int flag, bool state);
native bool     MarkerGetMatchFlag (marker m, int flag);

native void     MarkerSetMismatchFlag (marker m, int flag, bool state);
native bool     MarkerGetMismatchFlag (marker m, int flag);

//--------------------------------------------------------------------------------------------------
// Math
//--------------------------------------------------------------------------------------------------
// General math
native fixed SquareRoot (fixed x);
native fixed Pow (fixed x, fixed power);
native fixed ModF (fixed x, fixed m);
native fixed MinF (fixed x1, fixed x2);
native fixed MaxF (fixed x1, fixed x2);
native fixed AbsF (fixed x);

native int   ModI (int x, int m);
native int   MinI (int x1, int x2);
native int   MaxI (int x1, int x2);
native int   AbsI (int x);

// Trigonometry
// Note: All angles are in degrees
native fixed Sin (fixed degrees);
native fixed Cos (fixed degrees);
native fixed Tan (fixed degrees);
native fixed ASin (fixed x);
native fixed ACos (fixed x);
native fixed ATan (fixed x);
native fixed ATan2 (fixed y, fixed x);

// Random
// Note: Bounds are inclusive
//
native int RandomInt (int min, int max);
native fixed RandomFixed (fixed min, fixed max);
bool OneIn (int value) { return RandomInt(1, value) == 1; }

//--------------------------------------------------------------------------------------------------
// Melee
//
// Game initialization and configuration associated with standard melee games
//--------------------------------------------------------------------------------------------------
native void     MeleeInitResourcesForPlayer (int player, string race);
native void     MeleeInitResources ();

native void     MeleeInitUnitsForPlayer (int player, string race, point position);
native void     MeleeInitUnits ();

native void     MeleeInitAI ();

const int c_meleeOptionReveal   = 0;
const int c_meleeOptionDefeat   = 1;
const int c_meleeOptionVictory  = 2;

native void     MeleeSetOption (int player, int option, bool value);
native bool     MeleeGetOption (int player, int option);
native void     MeleeInitOptions ();

//--------------------------------------------------------------------------------------------------
// Mercenary
//--------------------------------------------------------------------------------------------------
const int c_invalidMercenaryId                = 0;

const int c_mercenaryStateEnabled           = 0;
const int c_mercenaryStateDisabled          = 1;
const int c_mercenaryStatePurchased         = 2;
const int c_mercenaryStateHidden            = 3;
const int c_mercenaryStateNew               = 4;

native int      MercenaryCreate (playergroup inPlayerGroup, int inState);
native int      MercenaryLastCreated ();
native void     MercenaryDestroy (int inMercenaryId);

native void     MercenarySetPlayerGroup (int inMercenaryId, playergroup inPlayerGroup);
native void     MercenarySetState (int inMercenaryId, int inState);
native void     MercenarySetCost (int inMercenaryId, int inCost);
native void     MercenarySetTitleText (int inMercenaryId, text inText);
native void     MercenarySetDescriptionText (int inMercenaryId, text inText);
native void     MercenarySetCostText (int inMercenaryId, text inText);
native void     MercenarySetUnitText (int inMercenaryId, text inText);
native void     MercenarySetAvailabilityText (int inMercenaryId, text inText);
native void     MercenarySetSpecialText (int inMercenaryId, text inText);
native void     MercenarySetTooltipText (int inMercenaryId, text inText);
native void     MercenarySetModelLink (int inMercenaryId, string inModelLink);
native void     MercenarySetScenePath (int inMercenaryId, string inFilePath);
native void     MercenarySetImageFilePath (int inMercenaryId, string inFilePath);
native void     MercenarySetRecentlyPurchased (int inMercenaryId, bool inRecent);
native void     MercenaryPurchase (int inMercenaryId);

native bool     MercenaryIsRecentlyPurchased (int inMercenaryId);

native void     MercenarySetSelected (playergroup inPlayerGroup, int inMercenaryId);
native int      MercenaryGetSelected (int player);

// Mercenary events
native void     TriggerAddEventMercenaryPanelExit (trigger t, int player);
native void     TriggerAddEventMercenaryPanelPurchase (trigger t, int player);
native void     TriggerAddEventMercenaryPanelSelectionChanged (trigger t, int player, int inMercenaryId);

//--------------------------------------------------------------------------------------------------
// Minimap
//--------------------------------------------------------------------------------------------------
native void     MinimapPing (playergroup players, point position, fixed duration, color c);

//--------------------------------------------------------------------------------------------------
// Misc
//--------------------------------------------------------------------------------------------------
native void    PerfTestStart (text name);
native void    PerfTestStop ();
native void    PerfTestGetFPS ();
native void    UnitStatsStart (text name, text unitName, text unitFood);
native void    UnitStatsStop ();
native void    EngineReset ();

//--------------------------------------------------------------------------------------------------
// Movie
//--------------------------------------------------------------------------------------------------
native void     MoviePlayAfterGame (playergroup players, string assetLink);

native void     TriggerAddEventMovieStarted (trigger t, int player);
native void     TriggerAddEventMovieFinished (trigger t, int player);
native void     TriggerAddEventMovieFunction (trigger t, int player, string functionName);

//--------------------------------------------------------------------------------------------------
// Objectives
//--------------------------------------------------------------------------------------------------
const int c_invalidObjectiveId            = 0;

const int c_primaryObjectivesId            = 1;
const int c_secondaryObjectivesId        = 2;

const int c_objectiveStateUnknown       = -1;
const int c_objectiveStateHidden        = 0;
const int c_objectiveStateActive        = 1;
const int c_objectiveStateCompleted     = 2;
const int c_objectiveStateFailed        = 3;

native int    ObjectiveCreate3 (
                text inName,
                text inDescription,
                int inState,
                bool inVisible,
                bool inPrimary
            );

native int          ObjectiveLastCreated ();
native void         ObjectiveDestroy (int inObjective);
native void         ObjectiveDestroyAll (playergroup inPlayers);

native void         ObjectiveShow (int inObjective, playergroup inPlayers, bool inShow);
native bool         ObjectiveVisible (int inObjective, int inPlayer);

native void         ObjectiveSetName (int inObjective, text inName);
native text         ObjectiveGetName (int inObjective);

native void         ObjectiveSetDescription (int inObjective, text inText);
native text         ObjectiveGetDescription (int inObjective);

native void         ObjectiveSetState (int inObjective, int inState);
native int          ObjectiveGetState (int inObjective);

native void         ObjectiveSetPlayerGroup (int inObjective, playergroup inPlayers);
native playergroup  ObjectiveGetPlayerGroup (int inObjective);

native void         ObjectiveSetPrimary (int inObjective, bool inPrimary);
native bool         ObjectiveGetPrimary (int inObjective);

//--------------------------------------------------------------------------------------------------
// Orders
// - Use a "null" abilcmd for "smart" orders (automatically determine ability based on target)
//--------------------------------------------------------------------------------------------------
native abilcmd  AbilityCommand (string inAbil, int inCmdIndex);
native string   AbilityCommandGetAbility (abilcmd inAbilCmd);
native int      AbilityCommandGetCommand (abilcmd inAbilCmd);

// - Command "action" type, which can be used to determine if the command needs a target or not
//
const int c_cmdActionNone       = 0;
const int c_cmdActionInstant    = 1;
const int c_cmdActionTarget     = 2;

native int      AbilityCommandGetAction (abilcmd inAbilCmd);

native order    Order (abilcmd inAbilCmd);
native order    OrderTargetingPoint (abilcmd inAbilCmd, point inPoint);
native order    OrderTargetingRelativePoint (abilcmd inAbilCmd, point inPoint);
native order    OrderTargetingUnit (abilcmd inAbilCmd, unit inUnit);
native order    OrderTargetingUnitGroup (abilcmd inAbilCmd, unitgroup inUnitGroup);
native order    OrderTargetingItem (abilcmd inAbilCmd, unit inItem);
native order    OrderSetAutoCast (abilcmd inAbilCmd, bool inAutoCastOn);

native void     OrderSetAbilityCommand (order inOrder, abilcmd inAbilCmd);
native abilcmd  OrderGetAbilityCommand (order inOrder);

native void     OrderSetPlayer (order inOrder, int inPlayer);
native int      OrderGetPlayer (order inOrder);

const int c_orderTargetNone         = 0;
const int c_orderTargetPoint        = 1;
const int c_orderTargetUnit         = 2;
const int c_orderTargetItem         = 3;

native int      OrderGetTargetType (order inOrder);

native bool     OrderSetTargetPlacement (order inOrder, point inPoint, unit inPlacer, string inType);
native void     OrderSetTargetPoint (order inOrder, point inPoint);
native point    OrderGetTargetPoint (order inOrder);
native point    OrderGetTargetPosition (order inOrder); // doesn't care what the target type is

native void     OrderSetTargetUnit (order inOrder, unit inUnit);
native unit     OrderGetTargetUnit (order inOrder);
native void     OrderSetTargetPassenger (order inOrder, unit inUnit);

native void     OrderSetTargetItem (order inOrder, unit inItem);
native unit     OrderGetTargetItem (order inOrder);

native void     OrderSetFlag (order inOrder, int inFlag, bool inValue);
native bool     OrderGetFlag (order inOrder, int inFlag);

//--------------------------------------------------------------------------------------------------
// Ping
//--------------------------------------------------------------------------------------------------
const int c_invalidPingId = 0;

native int         PingCreate (
                        playergroup players,
                        string modelLink,
                        point position,
                        color intColor,
                        fixed duration
                    );
native int          PingLastCreated ();
native void         PingDestroy (int p);
native void         PingDestroyAll ();

native void         PingSetPlayerGroup (int p, playergroup playerMask);
native playergroup  PingGetPlayerGroup (int p);

native void         PingSetModel (int p, string modelLink);

native void         PingSetPosition (int p, point position);
native point        PingGetPosition (int p);

native void         PingSetScale (int p, fixed scale);
native fixed        PingGetScale (int p);

native void         PingSetRotation (int p, fixed rotation);
native fixed        PingGetRotation (int p);

native void         PingSetColor (int p, color intColor);
native color        PingGetColor (int p);

native void         PingSetDuration (int p, fixed duration);
native fixed        PingGetDuration (int p);

native void         PingSetUnit (int p, unit u);
native unit         PingGetUnit (int p);

native void         PingSetTooltip (int p, text tooltip);
native text         PingGetTooltip (int p);

native void         PingSetVisible (int p, bool isVisible);
native bool         PingIsVisible (int p);

//--------------------------------------------------------------------------------------------------
// Planet
//--------------------------------------------------------------------------------------------------
const int c_invalidPlanetId     = 0;

const int c_planetStateHidden        = 0;
const int c_planetStateActive        = 1;
const int c_planetStateEmphasized   = 2;

const int c_planetPanelContactButtonStateDisabled = 0;
const int c_planetPanelContactButtonStateBlinking = 1;
const int c_planetPanelContactButtonStatePlay = 2;
const int c_planetPanelContactButtonStatePause = 3;

native int          PlanetCreate (playergroup inPlayerGroup, int inState);
native int          PlanetLastCreated ();

native void         PlanetDestroy (int inPlanetId);
native void         PlanetDestroyAll (playergroup inPlayerGroup);

native void         PlanetSetSelected (playergroup inPlayerGroup, int inPlanetId);
native int          PlanetGetSelected (int player);
native void         PlanetClearSelected (playergroup inPlayerGroup);

native void         PlanetSetPlayerGroup (int inPlanetId, playergroup inPlayerGroup);
native void         PlanetSetState (int inPlanetId, int inState);
native void         PlanetSetPlanetName (int inPlanetId, text inText);
native void         PlanetSetDescriptionText (int inPlanetId, text inText);
native void         PlanetSetTooltipText (int inPlanetId, text inText);
native void         PlanetSetContactTooltipText (int inPlanetId, text inText);
native void         PlanetSetTechnologyTooltipText (int inPlanetId, text inText);
native void         PlanetSetMissionTitle (int inPlanetId, text inText);
native void         PlanetSetMissionName (int inPlanetId, text inText);
native void         PlanetSetPrimaryObjectiveTitle (int inPlanetId, text inText);
native void         PlanetSetPrimaryObjectiveText (int inPlanetId, text inText);
native void         PlanetSetSecondaryObjectiveTitle (int inPlanetId, text inText);
native void         PlanetSetSecondaryObjectiveText (int inPlanetId, text inText);
native void         PlanetSetRewardTitle (int inPlanetId, text inText);
native void         PlanetSetRewardText (int inPlanetId, text inText);
native void         PlanetSetResearchTitle (int inPlanetId, text inText);
native void         PlanetSetResearchText (int inPlanetId, text inText);
native void         PlanetSetBonusTitle (int inPlanetId, text inText);
native void         PlanetSetBonusText (int inPlanetId, text inText);
native void         PlanetSetPlanetText (int inPlanetId, text inText);
native void         PlanetSetTechnologyTitle (int inPlanetId, text inText);
native void         PlanetSetTechnologyName (int inPlanetId, text inText);
native void         PlanetSetTechnologyText (int inPlanetId, text inText);
native void         PlanetSetContactTitle (int inPlanetId, text inText);
native void         PlanetSetContactName (int inPlanetId, text inText);
native void         PlanetSetContactModelLink (int inPlanetId, string inContactModelLink);
native void         PlanetSetBackgroundModelLink (int inPlanetId, string inBackgroundModelLink);
native void         PlanetSetPlanetModelLink (int inPlanetId, string inPlanetModelLink);
native void         PlanetSetTechnologyIconFilePath (int inPlanetId, string inTechnologyIconFilePath);
native void         PlanetSetTechnologyUnitLink (int inPlanetId, string inUnitLink);

native void         PlanetPanelSetContactButtonState (playergroup players, int inState);
native int          PlanetPanelGetContactButtonState (int player);
native void         PlanetPanelSetBackButtonEnabled (playergroup players, bool inEnabled);
native void         PlanetPanelSetBackgroundImage (playergroup players, string inFilePath);

// Planet events
native void         TriggerAddEventPlanetMissionLaunched (trigger t, int player);
native void         TriggerAddEventPlanetMissionSelected (trigger t, int player, int planetId);
native void         TriggerAddEventPlanetPanelCanceled (trigger t, int player);
native void         TriggerAddEventPlanetPanelReplayPressed (trigger t, int player);
native void         TriggerAddEventPlanetPanelBirthComplete (trigger t, int player);
native void         TriggerAddEventPlanetPanelDeathComplete (trigger t, int player);

native int          EventPlanetPanelMissionSelected ();
native int          EventPlanetPanelDifficultySelected ();

//--------------------------------------------------------------------------------------------------
// Victory Panel
//--------------------------------------------------------------------------------------------------
native void         VictoryPanelSetMissionTitle (text inText);
native void         VictoryPanelSetMissionText (text inText);
native void         VictoryPanelSetMissionTimeTitle (text inText);
native void         VictoryPanelSetMissionTimeText (text inText);
native void         VictoryPanelSetRewardTitle (text inText);
native void         VictoryPanelSetRewardText (text inText);
native void         VictoryPanelSetRewardCredits (int inCredits);
native void         VictoryPanelSetAchievementsTitle (text inText);
native void         VictoryPanelSetStatisticsTitle (text inText);
native void         VictoryPanelSetCustomStatisticText (text inText);
native void         VictoryPanelSetCustomStatisticValue (text inText);
native void         VictoryPanelSetPlanetModelLink (string inModelLink);
native void         VictoryPanelAddCustomStatisticLine (text inText, text inValueText);
native void         VictoryPanelClearCustomStatisticTable ();
native void         VictoryPanelAddTrackedStatistic (string inStatistic);
native void         VictoryPanelAddAchievement (string inAchievement);

native void         TriggerAddEventVictoryPanelExit (trigger t, int player);
native void         TriggerAddEventVictoryPanelPlayMissionAgain (trigger t, int player);

native int          EventVictoryPanelDifficultySelected ();

//--------------------------------------------------------------------------------------------------
// Help
//--------------------------------------------------------------------------------------------------
const int c_helpPanelPageTips = 0;
const int c_helpPanelPageTutorials = 1;

native void            HelpPanelAddTip (playergroup players, text titleText, text descriptionText, text alertText, string iconPath);
native void            HelpPanelAddHint (playergroup players, text titleText, text descriptionText, string iconPath);
native void            HelpPanelAddTutorial (playergroup players, text titleText, text descriptionText, string iconPath, string moviePath, bool flashing);
native void         HelpPanelDisplayPage (playergroup players, int inPage);
native void         HelpPanelEnableTechTreeButton (playergroup inPlayerGroup, bool inEnable);
native void         HelpPanelEnableTechGlossaryButton (playergroup inPlayerGroup, bool inEnable);
native void         HelpPanelShowTechTreeRace (playergroup inPlayerGroup, string inRace, bool inShow);

native void         TipAlertPanelClear (playergroup inPlayerGroup);

//--------------------------------------------------------------------------------------------------
// Room
//--------------------------------------------------------------------------------------------------
const int c_invalidRoomId = 0;

native void         RoomPanelSetPositionUnit (unit inUnit);

native void            RoomPanelAddButton (playergroup players, text buttonText, string iconPath);
native int          RoomPanelLastAdded ();

native void            RoomPanelRemoveButton (int roomId);
native void            RoomPanelRemoveAllButtons (playergroup players);
native void         RoomPanelShowRoomButton (int roomId, playergroup players, bool isVisible);
native void         RoomPanelSetButtonPressedIcon (int inRoomId, string inPressedIconPath);
native void         RoomPanelSetButtonEnabled (int inRoomId, bool inEnabled);

// Room events
native void         TriggerAddEventRoomPanelPressedButton (trigger t, int player, int roomId);
native void         TriggerAddEventRoomPanelCanceled (trigger t, int player);

native void         TriggerAddEventRoomPanelMouseRollOver (trigger t, int player);
native void         TriggerAddEventRoomPanelMouseRollOut (trigger t, int player);

native int          EventRoomPanelButtonPressed ();

//--------------------------------------------------------------------------------------------------
// Players
//--------------------------------------------------------------------------------------------------
const int c_playerAny = -1;
const int c_maxPlayers = 32;

// Player properties
const int c_playerPropMinerals              = 0;
const int c_playerPropVespene               = 1;
const int c_playerPropTerrazine             = 2;
const int c_playerPropCustom                = 3;
const int c_playerPropSuppliesUsed          = 4;
const int c_playerPropSuppliesMade          = 5;
const int c_playerPropSuppliesLimit         = 6;
const int c_playerPropCredits               = 7;
const int c_playerPropCreditsSpent          = 8;
const int c_playerPropResearchPoints        = 9;
const int c_playerPropResearchPointsSpent   = 10;
const int c_playerPropHandicap              = 11;
const int c_playerPropMineralsCollected     = 12;
const int c_playerPropVespeneCollected      = 13;
const int c_playerPropTerrazineCollected    = 14;
const int c_playerPropCustomCollected       = 15;

// Player property operations
const int c_playerPropOperSetTo         = 0;
const int c_playerPropOperAdd           = 1;
const int c_playerPropOperSubtract      = 2;

native void     PlayerModifyPropertyInt (int inPlayer, int inProp, int inOper, int inVal);
native void     PlayerModifyPropertyFixed (int inPlayer, int inProp, int inOper, fixed inVal);

native int      PlayerGetPropertyInt (int inPlayer, int inProp);
native fixed    PlayerGetPropertyFixed (int inPlayer, int inProp);

// Player status
const int c_playerStatusUnused          = 0;    // No player in this slot
const int c_playerStatusActive          = 1;    // Player is actively playing
const int c_playerStatusLeft            = 2;    // Player has left the game

native int      PlayerStatus (int inPlayer);
native int      PlayerType (int inPlayer); // Returns c_playerType<*> value
native text     PlayerName (int inPlayer);
native string   PlayerRace (int inPlayer);
native int      PlayerDifficulty (int inPlayer);
native void     PlayerSetDifficulty (int inPlayer, int inDifficulty);
native point    PlayerStartLocation (int inPlayer);

native void     PlayerSetColorIndex (int inPlayer, int inIndex, bool inChangeUnits);
native int      PlayerGetColorIndex (int inPlayer, bool inDefault);
native text     PlayerColorName (int inColor);

native void PlayerSetAlliance (int inSourcePlayer, int inAllianceId, int inTargetPlayer, bool ally);
native bool PlayerGetAlliance (int inSourcePlayer, int inAllianceId, int inTargetPlayer);

// Player states
const int c_playerStateShowScore            = 0;
const int c_playerStateXPGain                = 1;
const int c_playerStateAbortEnabled            = 2;
const int c_playerStateRestartEnabled        = 3;
const int c_playerStateContinueEnabled        = 4;
const int c_playerStateShowWorldTip            = 5;
const int c_playerStateFidgetingEnabled        = 6;
const int c_playerStateDisplayInLeaderPanel = 7;
const int c_playerStateDisplayInViewMenu    = 8;

native void PlayerSetState (int inPlayer, int inState, bool inVal);
native bool PlayerGetState (int inPlayer, int inState);

native void PlayerBeaconClearTarget (int inPlayer, int inBeacon);
native bool PlayerBeaconIsAutoCast (int inPlayer, int inBeacon);
native bool PlayerBeaconIsFromUser (int inPlayer, int inBeacon);
native bool PlayerBeaconIsSet (int inPlayer, int inBeacon);
native point PlayerBeaconGetTargetPoint (int inPlayer, int inBeacon);
native unit PlayerBeaconGetTargetUnit (int inPlayer, int inBeacon);
native void PlayerBeaconSetAutoCast (int inPlayer, int inBeacon, bool enable);
native void PlayerBeaconSetTargetPoint (int inPlayer, int inBeacon, point inPoint, bool alert);
native void PlayerBeaconSetTargetUnit (int inPlayer, int inBeacon, unit inUnit, bool alert);
native void PlayerBeaconAlert (int inPlayer, int inBeacon, string inAlert, text inMessage);

native void PlayerPauseAllCharges (int inPlayer, bool inPause);
native void PlayerPauseAllCooldowns (int inPlayer, bool inPause);

native void PlayerAddChargeRegen (int inPlayer, string inCharge, fixed inVal);
native fixed PlayerGetChargeRegen (int inPlayer, string inCharge);
native void PlayerAddChargeUsed (int inPlayer, string inCharge, fixed inVal);
native fixed PlayerGetChargeUsed (int inPlayer, string inCharge);

native void PlayerAddCooldown (int inPlayer, string inCooldown, fixed inVal);
native fixed PlayerGetCooldown (int inPlayer, string inCooldown);

native void PlayerCreateEffectPoint (int inPlayer, string inEffect, point inTarget);
native void PlayerCreateEffectUnit (int inPlayer, string inEffect, unit inTarget);
native int PlayerValidateEffectPoint (int inPlayer, string inEffect, point inTarget);
native int PlayerValidateEffectUnit (int inPlayer, string inEffect, unit inTarget);

// Player scores
native void PlayerScoreValueEnable (int player, string value, bool enable);
native fixed PlayerScoreValueGetAsFixed (int player, string value);
native int PlayerScoreValueGetAsInt (int player, string value);
native void PlayerScoreValueSetFromFixed (int player, string value, fixed amount);
native void PlayerScoreValueSetFromInt (int player, string value, int amount);

// Player events
const int c_gameResultUndecided     = 0;
const int c_gameResultVictory       = 1;
const int c_gameResultDefeat        = 2;
const int c_gameResultTie           = 3;

native void TriggerAddEventPlayerAllianceChange (trigger inTrigger, int player);
native void TriggerAddEventPlayerLeft (trigger inTrigger, int player, int inResult);
native void TriggerAddEventPlayerPropChange (trigger inTrigger, int player, int inProp);
native void TriggerAddEventPlayerAIWave (trigger inTrigger, int player);

native int  EventPlayer ();
native int  EventPlayerProperty ();

// Difficulty info
native text DifficultyName (int inDifficulty);
native text DifficultyNameCampaign (int inDifficulty);
native bool DifficultyEnabled (int inDifficulty);
native int  DifficultyAPM (int inDifficulty);   // AI Actions Per Minute

//--------------------------------------------------------------------------------------------------
// Player Groups
//--------------------------------------------------------------------------------------------------
native playergroup PlayerGroupEmpty ();
native playergroup PlayerGroupCopy (playergroup inGroup);
native playergroup PlayerGroupAll ();
native playergroup PlayerGroupActive ();
native playergroup PlayerGroupSingle (int inPlayer);

const int c_playerGroupAlly     = 0;    // Allied players of the given player
const int c_playerGroupEnemy    = 1;    // Enemy players of the given player
const int c_playerGroupAny      = 2;    // Any player.

native playergroup PlayerGroupAlliance (int inType, int inPlayer);

native void PlayerGroupClear (playergroup inGroup);
native void PlayerGroupAdd (playergroup inGroup, int inPlayer);
native void PlayerGroupRemove (playergroup inGroup, int inPlayer);

native int  PlayerGroupCount (playergroup inGroup);
native int  PlayerGroupPlayer (playergroup inGroup, int inIndex);
native bool PlayerGroupHasPlayer (playergroup inGroup, int inPlayer);

//--------------------------------------------------------------------------------------------------
// Points
//--------------------------------------------------------------------------------------------------
native point Point (fixed x, fixed y);
native point PointWithOffset (point p, fixed x, fixed y);
native point PointWithOffsetPolar (point p, fixed distance, fixed angle);
native point PointFromId (int id);

native fixed PointGetX (point p);
native fixed PointGetY (point p);
native void  PointSet (point p1, point p2);

native fixed PointGetFacing (point p);
native void  PointSetFacing (point p, fixed inFacing);

native fixed PointGetHeight (point p);
native void  PointSetHeight (point p, fixed inHeight);

native bool  PointsInRange (point p1, point p2, fixed range);

native fixed AngleBetweenPoints (point p1, point p2);
native fixed DistanceBetweenPoints (point p1, point p2);

native int   PointPathingCost (point p1, point p2);
native fixed PointPathingCliffLevel (point p);
native bool  PointPathingPassable (point p);
native bool  PointPathingIsConnected (point p1, point p2);
native point PointReflect (point source, point dest, fixed angle);

//--------------------------------------------------------------------------------------------------
// Portrait
//--------------------------------------------------------------------------------------------------
const int c_noPortraitChannel      = -1;
const int c_invalidPortraitId       = 0;

//These need to match the ERenderType enum in CFrame.h
const int c_portraitRenderTypeHDR = 0;
const int c_portraitRenderTypeLDR = 1;

native int          PortraitCreate (
                        int offsetX,
                        int offsetY,
                        int anchor,
                        int width,
                        int height,
                        string modelLink,
                        string cameraLink,
                        string animProps,
                        bool visible,
                        bool waitUntilLoaded
                    );
native int          PortraitLastCreated ();
native void         PortraitDestroy (int p);
native void         PortraitDestroyAll ();
native void         PortraitWaitForLoad (int p);

native int          PortraitGetGame ();
native int          PortraitGetPlanetPanel ();
native void         PortraitSetModel (int p, string modelLink, bool waitUntilLoaded);
native void         PortraitSetAnim (int p, string anim);
native void         PortraitSetCamera (int p, string cameraName);
native void         PortraitSetSize (int p, int width, int height);
native void         PortraitSetPosition (int p, int anchor, int offsetX, int offsetY);
native void         PortraitSetFullscreen (int p, bool isFullscreen);
native void         PortraitSetOffscreen (int p, bool isOffscreen);
native void         PortraitSetVisible (int p, playergroup players, bool isVisible, bool forceVisible);
native void         PortraitSetBorderVisible (int p, bool isBorderVisible);
native void         PortraitSetBorderTexture (int p, string inBorderFilePath);
native void         PortraitSetBackgroundVisible (int p, bool isBackgroundVisible);
native void         PortraitSetChannel (int p, int channel);
native void         PortraitSetChannelPortrait (int p, int dest, int channel);
native void         PortraitUseTransition (int p, bool useTransition);
native void         PortraitSetRenderType (int p, int renderType);
native void         PortraitSetMuted (int p, bool isMuted);
native void         PortraitForceTransition (int p, bool isVisible, bool isInstant);
native void         PortraitSetPaused (int p, bool isPaused);
native void         PortraitSetTintColor (int p, color inColor);
native void         PortraitSetLight (int p, string lightLink);
native void         PortraitSetTransitionModel (int p, string modelLink);

native bool         PortraitVisible (int p, int player);

//--------------------------------------------------------------------------------------------------
// Preload
//--------------------------------------------------------------------------------------------------
native void PreloadAsset (string key, bool queue);
native void PreloadImage (string path, bool queue);
native void PreloadModel (string path, bool queue);
native void PreloadMovie (string path, bool queue);
native void PreloadObject (int catalog, string id, bool queue);
native void PreloadScene (string path, bool queue);
native void PreloadScript (string path, bool queue);
native void PreloadSound (string path, bool queue);

//--------------------------------------------------------------------------------------------------
// Purchase
//--------------------------------------------------------------------------------------------------
const int c_invalidPurchaseItemId            = 0;
const int c_invalidPurchaseCategoryId       = 0;
const int c_invalidPurchaseGroupId            = 0;

const int c_purchaseCategoryStateEnabled    = 0;
const int c_purchaseCategoryStateDisabled    = 1;
const int c_purchaseCategoryStateHidden        = 2;
const int c_purchaseCategoryStateNew        = 3;

const int c_purchaseGroupStateEnabled       = 0;
const int c_purchaseGroupStateDisabled      = 1;
const int c_purchaseGroupStateHidden        = 2;
const int c_purchaseGroupStateNew           = 3;

const int c_purchaseItemStateEnabled        = 0;
const int c_purchaseItemStateDisabled        = 1;
const int c_purchaseItemStatePurchased        = 2;
const int c_purchaseItemStateHidden            = 3;
//--------------------------------------------------------------------------------------------------
native int              PurchaseCategoryCreate (playergroup inPlayerGroup, int inSlot);
native int              PurchaseCategoryLastCreated ();

native void             PurchaseCategoryDestroy (int inPurchaseCategoryId);
native void             PurchaseCategoryDestroyAll (playergroup inPlayerGroup);

native void             PurchaseCategorySetPlayerGroup (int inPurchaseCategoryId, playergroup inPlayerGroup);
native void             PurchaseCategorySetNameText (int inPurchaseCategoryId, text inText);
native void             PurchaseCategorySetState (int inPurchaseCategoryId, int inState);
native void             PurchaseCategorySetSlot (int inPurchaseCategoryId, int inSlot);

native void             PurchaseSetSelectedPurchaseCategory (playergroup inPlayerGroup, int inPurchaseCategoryId);
native int              PurchaseGetSelectedPurchaseCategory (int inPlayer);
//--------------------------------------------------------------------------------------------------
native int              PurchaseGroupCreate (playergroup inPlayerGroup, int inPurchaseCategoryId, int inSlot);
native int              PurchaseGroupLastCreated ();

native void             PurchaseGroupDestroy (int inPurchaseGroupId);
native void             PurchaseGroupDestroyAll (playergroup inPlayerGroup);

native void             PurchaseGroupSetPlayerGroup (int inPurchaseGroupId, playergroup inPlayerGroup);
native void             PurchaseGroupSetNameText (int inPurchaseGroupId, text inText);
native void             PurchaseGroupSetTooltipText (int inPurchaseGroupId, text inText);
native void             PurchaseGroupSetIconFilePath (int inPurchaseGroupId, string inFilePath);
native void             PurchaseGroupSetState (int inPurchaseGroupId, int inState);
native void             PurchaseGroupSetSlot (int inPurchaseGroupId, int inSlot);
native void             PurchaseGroupSetUnitLink (int inPurchaseGroupId, string inUnitLink);

//--------------------------------------------------------------------------------------------------
native int              PurchaseItemCreate (playergroup inPlayerGroup, int inPurchaseGroupId, int inSlot);
native int              PurchaseItemLastCreated ();

native void             PurchaseItemDestroy (int inPurchaseItemId);
native void             PurchaseItemDestroyAll (playergroup inPlayerGroup);

native void             PurchaseItemSetPlayerGroup (int inPurchaseItemId, playergroup inPlayerGroup);
native void             PurchaseItemSetNameText (int inPurchaseItemId, text inText);
native void             PurchaseItemSetTooltipText (int inPurchaseItemId, text inText);
native void             PurchaseItemSetDescriptionText (int inPurchaseItemId, text inText);
native void             PurchaseItemSetIconFilePath (int inPurchaseItemId, string inFilePath);
native void             PurchaseItemSetMovieFilePath (int inPurchaseItemId, string inFilePath);
native void             PurchaseItemSetCost (int inPurchaseItemId, int inCost);
native void             PurchaseItemSetState (int inPurchaseItemId, int inState);
native void             PurchaseItemSetRecentlyPurchased (int inPurchaseItemId, bool inRecent);
native void             PurchaseItemSetSlot (int inPurchaseItemId, int inSlot);
native void             PurchaseItemPurchase (int inPurchaseItemId);

native bool             PurchaseItemIsRecentlyPurchased (int inPurchaseItemId);

native void             PurchaseSetSelectedPurchaseItem (playergroup inPlayerGroup, int inPurchaseItemId);
native int              PurchaseGetSelectedPurchaseItem (int inPlayer);
//--------------------------------------------------------------------------------------------------
// Purchase events
native void             TriggerAddEventPurchaseMade (trigger t, int player, int inPurchaseItemId);
native void             TriggerAddEventPurchaseExit (trigger t, int player);
native void             TriggerAddEventSelectedPurchaseItemChanged (trigger t, int player, int inPurchaseItemId);
native void             TriggerAddEventSelectedPurchaseCategoryChanged (trigger t, int player, int inPurchaseCategoryId);

native int              EventPurchaseMade ();

//--------------------------------------------------------------------------------------------------
// Regions
//
// Regions are comprised of any number of basic shapes (rectangles, circles, etc).  Each shape
// can be specified as positive or negative.  A point is considered within the region if it is
// within a least one positive shape but not within any negative shapes.
//
// Example:
// An L-shaped region could be created with either
//
// (a) Two positive rectangles
//      _
//     | | <-- (+)
//     | |
//     |_|___
//     |_____| <-- (+)
//
// (b) One positive and one negative rectangle
//        _____
//      _|___  |
//     | |   | |
//     | |   | | <-- (-)
//     | |___|_|
//     |_____|   <-- (+)
//
// The "bounds" of the region are defined as the smallest rectangle which contains all positive
// shapes.  The "center" is the average of the center of all positive shapes, weighted by area.
//
// The "offset" of the region is relative to the shape coordinates.  The center and bounds take
// the offset into account.
//
//--------------------------------------------------------------------------------------------------
native region RegionEmpty ();
native region RegionEntireMap ();
native region RegionPlayableMap ();
native void RegionPlayableMapSet (region r);

native region RegionRect (fixed minx, fixed miny, fixed maxx, fixed maxy);
native region RegionCircle (point center, fixed radius);
native region RegionFromId (int id);

native void RegionAddRect (region r, bool positive, fixed minx, fixed miny, fixed maxx, fixed maxy);
native void RegionAddCircle (region r, bool positive, point center, fixed radius);
native void RegionAddRegion (region r, region regToAdd);

native void  RegionSetOffset (region r, point offset);
native point RegionGetOffset (region r);

native bool RegionContainsPoint (region r, point p);

// RegionRandomPoint attempts to find a random point somewhere within the region.  For regions
// containing only a single positive shape, this is guaranteed to work, but for more complex
// sets of shapes it may give up after a maximum number of tries and return (0, 0).
//
native point RegionRandomPoint (region r);

native point RegionGetBoundsMin (region r);
native point RegionGetBoundsMax (region r);
native point RegionGetCenter (region r);

// Setting the center will adjust the offset such that
// the region becomes centered on the given point.
//
native void RegionSetCenter (region r, point p);

// RegionAttachToUnit will attach the region to the given unit such that the region center
// is always at the unit's position plus the given offset.  Use a null unit to detach the region
// from whatever unit it may already be attached to.
//
native void RegionAttachToUnit (region r, unit u, point offset);
native unit RegionGetAttachUnit (region r);

//--------------------------------------------------------------------------------------------------
// Research
//--------------------------------------------------------------------------------------------------
const int c_invalidResearchCategoryId            = 0;
const int c_invalidResearchTierId                = 0;
const int c_invalidResearchItemId                = 0;

const int c_researchItemStateEnabled            = 0;
const int c_researchItemStateDisabled           = 1;
const int c_researchItemStatePurchased          = 2;
//--------------------------------------------------------------------------------------------------
native int          ResearchCategoryCreate (playergroup inPlayerGroup, int inSlot);
native int          ResearchCategoryLastCreated ();

native void         ResearchCategoryDestroy (int inResearchCategoryId);
native void         ResearchCategoryDestroyAll (playergroup inPlayerGroup);

native void         ResearchCategorySetPlayerGroup (int inResearchCategoryId, playergroup inPlayerGroup);
native void         ResearchCategorySetSlot (int inResearchCategoryId, int inSlot);
native void         ResearchCategorySetNameText (int inResearchCategoryId, text inText);
native void         ResearchCategorySetCurrentLevel (int inResearchCategoryId, int inLevel);
native void         ResearchCategorySetLastLevel (int inResearchCategoryId, int inLevel);
//--------------------------------------------------------------------------------------------------
native int          ResearchTierCreate (playergroup inPlayerGroup, int inResearchCategoryId, int inSlot);
native int          ResearchTierLastCreated ();

native void         ResearchTierDestroy (int inResearchTierId);
native void         ResearchTierDestroyAll (playergroup inPlayerGroup);

native void         ResearchTierSetPlayerGroup (int inResearchTierId, playergroup inPlayerGroup);
native void         ResearchTierSetSlot (int inResearchTierId, int inSlot);
native void         ResearchTierSetRequiredLevel (int inResearchTierId, int inLevel);
native void         ResearchTierSetMaxPurchasesAllowed (int inResearchTierId, int inMax);
//--------------------------------------------------------------------------------------------------
native int          ResearchItemCreate (playergroup inPlayerGroup, int inResearchTierId, int inState);
native int          ResearchItemLastCreated ();

native void         ResearchItemDestroy (int inResearchItemId);
native void         ResearchItemDestroyAll (playergroup inPlayerGroup);

native void         ResearchItemSetPlayerGroup (int inResearchItemId, playergroup inPlayerGroup);
native void         ResearchItemSetSlot (int inResearchItemId, int inSlot);
native void         ResearchItemSetState (int inResearchItemId, int inState);
native void         ResearchItemSetNameText (int inResearchItemId, text inText);
native void         ResearchItemSetDescriptionText (int inResearchItemId, text inText);
native void         ResearchItemSetTooltipText (int inResearchItemId, text inText);
native void         ResearchItemSetConfirmationText (int inResearchItemId, text inText);
native void         ResearchItemSetIconFilePath (int inResearchItemId, string inFilePath);
native void         ResearchItemSetMovieFilePath (int inResearchItemId, string inFilePath);
native void         ResearchItemSetRecentlyPurchased (int inResearchItemId, bool inRecent);

native bool         ResearchItemIsRecentlyPurchased (int inResearchItemId);
native void            ResearchItemPurchase (int inResearchItemId);

native void         ResearchItemSetSelected (playergroup inPlayerGroup, int inResearchItemId);
native int          ResearchItemGetSelected (int inPlayer);

// Research events
native void         TriggerAddEventResearchPanelExit (trigger t, int player);
native void         TriggerAddEventResearchPanelPurchase (trigger t, int player);
native void         TriggerAddEventResearchPanelSelectionChanged (trigger t, int player, int inResearchItemId);

//--------------------------------------------------------------------------------------------------
// Sound
//
// - A specific sound index within the sound list may be specified.
//   Using c_soundIndexAny will choose a sound either sequentially or randomly,
//   based on the sound data.
//
// - Use a "null" playergroup to play the sound for all players.
//
// - 3d sounds are only played for players who currently have vision of the sound position.
//
// - Volumes are expressed as a percent (0-100) of maximum volume.
//--------------------------------------------------------------------------------------------------
const int c_soundIndexAny = -1;

// Sound Info
native soundlink SoundLink (string soundId, int soundIndex);
native string SoundLinkId (soundlink soundId);
native int SoundLinkAsset (soundlink soundId);

// Sound
native void SoundPlay (
    soundlink link,
    playergroup players,
    fixed volume,
    fixed offset
);
native void SoundPlayAtPoint (
    soundlink link,
    playergroup players,
    point inPoint,
    fixed height,
    fixed volume,
    fixed offset
);
native void SoundPlayOnUnit (
    soundlink link,
    playergroup players,
    unit inUnit,
    fixed height,
    fixed volume,
    fixed offset
);

// - SoundPlayScene attempts to synchronize unit animations with the sound duration
native void SoundPlayScene (
    soundlink link,
    playergroup players,
    unitgroup units,
    string animProps
);
native void SoundPlaySceneFile (
    soundlink link,
    playergroup players,
    string sceneFile,
    string camera
);

native sound    SoundLastPlayed ();

native void     SoundStop (sound s, bool fade);
native void     SoundStopAllModelSounds ();
native void     SoundStopAllTriggerSounds (bool fade);

native void     SoundSetVolume (sound s, fixed volume);
native void     SoundSetPosition (sound s, point position, fixed height);

const int c_soundOffsetStart    = 0;
const int c_soundOffsetEnd      = 1;

native void     SoundSetOffset (sound s, fixed offset, int offsetType);
native void     SoundWait (sound s, fixed offset, int offsetType);
native void     SoundAttachUnit (sound s, unit u, fixed height);

// Sound Lengths
// - Note: Since sound files are localized and potentially different for each player,
//         a network query must be sent to all players, and all results must be
//         received before a synchronous result can be accessed.
//
//         SoundLengthQuery     - Initiate a network query for the given sound
//
//         SoundLengthQueryWait - Pause the current thread until all outstanding sound length
//                                query results have been synchronized
//
//         SoundLengthSync      - Retrieve the synchronized sound length result for the given
//                                sound
//
native void     SoundLengthQuery (soundlink info);
native void     SoundLengthQueryWait ();
native fixed    SoundLengthSync (soundlink info);

// Sound channels
native void     SoundChannelSetVolume (playergroup players, int channel, fixed volume, fixed duration);
native void     SoundChannelMute (playergroup players, int channel, bool mute);
native void     SoundChannelPause (playergroup players, int channel, bool pause);
native void     SoundChannelStop (playergroup players, int channel);

// Other properties
native void     SoundSetReverb (string inReverbLink, fixed inDuration, bool inAmbient, bool inGlobal);
native void     SoundSetFactors (fixed distance, fixed doppler, fixed rolloff);

native text     SoundSubtitleText (soundlink link);

//--------------------------------------------------------------------------------------------------
// Soundtracks
//--------------------------------------------------------------------------------------------------
const int c_soundtrackCueAny = -1;
const int c_soundtrackIndexAny = -1;

native void     SoundtrackDefault (playergroup players, int category, string soundtrack, int cue, int index);
native void     SoundtrackPlay (playergroup players, int category, string soundtrack, int cue, int index, bool makeDefault);
native void     SoundtrackPause (playergroup players, int category, bool pause, bool fade);
native void     SoundtrackSetContinuous (playergroup players, int category, bool continuous);
native void     SoundtrackSetDelay (playergroup players, int category, fixed delay);
native void     SoundtrackStop (playergroup players, int category, bool fade);
native void     SoundtrackWait (string soundtrack);

//--------------------------------------------------------------------------------------------------
// Story Mode
//--------------------------------------------------------------------------------------------------
native void     StoryMode (playergroup players, bool storyMode);     //  Blizzard maps only

//--------------------------------------------------------------------------------------------------
// Strings
//--------------------------------------------------------------------------------------------------
native int      StringLength (string s);
native string   StringCase (string s, bool upper);
native text     TextCase (text t, bool upper);
native string   StringSub (string s, int start, int end);

const bool c_stringCase = true;
const bool c_stringNoCase = false;

native bool     StringEqual (string s1, string s2, bool caseSens);

// StringFind returns the position of the first occurrence of s2 within s1,
// or c_stringNotFound if there isn't one.
//
const int c_stringNotFound  = -1;

native int      StringFind (string s1, string s2, bool caseSens);

// StringContains
const int c_stringBegin     = 0;    // True if s1 begins with s2
const int c_stringEnd       = 1;    // True if s1 ends with s2
const int c_stringAnywhere  = 2;    // True if s1 contains s2 anywhere

native bool     StringContains (string s1, string s2, int location, bool caseSens);

// StringWord splits the string into words separated by whitespace,
// and returns the word corresponding to the given index.
//
// Ex: StringWord("klaatu barada nikto", 2) will return "barada"
//
native string   StringWord (string s, int index);

// StringReplace replaces the indexed character range in the string with the replacement string
// and returns the result.
//
native string   StringReplace (string s, string replace, int start, int end);

// StringReplaceWord replaces occurrences of the given word in the string with the replacement
// string and returns the result.
//
const int c_stringReplaceAll    = -1;   // Use as maxCount for all occurrences

native string   StringReplaceWord (string s, string word, string replace, int maxCount, bool caseSens);
native text     TextReplaceWord (text t, text word, text replace, int maxCount, bool caseSens);

// StringExternal looks up the given string identifer in the externalized string table
native text     StringExternal (string s);

// StringToText converts the given string to text directly (not externalized)
native text     StringToText (string s);

// TextWithColor applies a formatting tag to the input text for the given color
native text     TextWithColor (text t, color c);

// TextTimeFormat converts a time value, in seconds, to text using a format string
// - The time format string should use the following tokens:
//
//      <hour/>         = hour count, rounded down
//      <hour2/>        = hour count, rounded down, always two digits
//      <hourtotal/>    = total hour count, rounded down
//      <hourtotaln/>   = total hour count, rounded to nearest
//      <hourtotalu/>   = total hour count, rounded up
//      <min/>          = minute count, rounded down
//      <min2/>         = minute count, rounded down, always two digits
//      <mintotal/>     = total minute count, rounded down
//      <mintotaln/>    = total minute count, rounded to nearest
//      <mintotalu/>    = total minute count, rounded up
//      <sec/>          = second count
//      <sec2/>         = second count, always two digits
//      <sectotal/>     = total second count
//      <short/>        = one of "<hour/>h", "<min/>m", or "<sec/>s"
//
//   Examples (these all represent the same value):
//      "<hour/>:<min2/>:<sec2/>"   ->  1:08:38     (this is the default format)
//      "<mintotaln/> minutes"      ->  69 minutes
//      "<mintotal/>:<sec2/>"       ->  68:38
//      "<short/>"                  ->  1h
//
native text     TextTimeFormat (text inFormat, int inSecs);

//--------------------------------------------------------------------------------------------------
// Tech Tree
//--------------------------------------------------------------------------------------------------
const int c_techCountQueuedOnly         = 0;
const int c_techCountQueuedOrBetter     = 1;    // Queued + In Progress + Complete
const int c_techCountInProgressOnly     = 2;
const int c_techCountInProgressOrBetter = 3;    // In Progress + Complete
const int c_techCountCompleteOnly       = 4;
const int c_techCountIncompleteOnly     = 5;    // Queued + In Progress

native void     TechTreeAbilityAllow (int player, abilcmd inAbilCmd, bool allow);
native int      TechTreeAbilityCount (int player, string abilType, int countType);
native bool     TechTreeAbilityIsAllowed (int player, abilcmd inAbilCmd);

native void     TechTreeBehaviorAllow (int player, string behaviorType, bool allow);
native int      TechTreeBehaviorCount (int player, string behaviorType, int countType);
native bool     TechTreeBehaviorIsAllowed (int player, string behaviorType);
native abilcmd  TechTreeBehaviorProducedAbilCmd (string behaviorType, int index);
native int      TechTreeBehaviorProducedAbilCmdCount (string behaviorType);

native void     TechTreeRequirementsEnable (int player, bool enable);
native bool     TechTreeRequirementsEnabled (int player);
native void     TechTreeRestrictionsEnable (int player, bool enable);
native bool     TechTreeRestrictionsEnabled (int player);

native void     TechTreeUnitAllow (int player, string unitType, bool allow);
native int      TechTreeUnitCount (int player, string unitType, int countType);
native int      TechTreeUnitAliasCount (int player, string unitType, int countType);
native int      TechTreeUnitCountEx (int player, string unitType, string equivType, int countType);
native bool     TechTreeUnitIsAllowed (int player, string unitType);
native void     TechTreeUnitHelp (int player, string unitType, bool display);
native void     TechTreeUnitHelpDefault (int player, bool display);
native abilcmd  TechTreeUnitProducedAbilCmd (string unitType, int index);
native int      TechTreeUnitProducedAbilCmdCount (string unitType);
native string   TechTreeUnitProducesUnit (string unitType, int index);
native int      TechTreeUnitProducesUnitCount (string unitType);
native string   TechTreeUnitProducesUpgrade (string unitType, int index);
native int      TechTreeUnitProducesUpgradeCount (string unitType);

native void     TechTreeUpgradeAddLevel (int player, string upgradeType, int levels);
native void     TechTreeUpgradeAllow (int player, string upgradeType, bool allow);
native int      TechTreeUpgradeCount (int player, string upgradeType, int countType);
native bool     TechTreeUpgradeIsAllowed (int player, string upgradeType);
native abilcmd  TechTreeUpgradeProducedAbilCmd (string upgradeType, int index);
native int      TechTreeUpgradeProducedAbilCmdCount (string upgradeType);

//--------------------------------------------------------------------------------------------------
// Text Tags
//--------------------------------------------------------------------------------------------------
const int c_textTagNone = 0;

// Edge
const int c_textTagEdgeTop      = 0;
const int c_textTagEdgeLeft     = 1;
const int c_textTagEdgeBottom   = 2;
const int c_textTagEdgeRight    = 3;

native int      TextTagCreate (
                    text inText,
                    int inFontSize,
                    point inPoint,
                    fixed inHeightOffset,
                    bool inShow,
                    bool useFogofWar,
                    playergroup inPlayers
                );
native int      TextTagLastCreated ();
native void     TextTagDestroy (int inTag);

native void     TextTagSetText (int inTag, text inText);
native void     TextTagSetTextShadow (int inTag, bool inVal);
native void     TextTagSetFontSize (int inTag, int inFontSize);
native void     TextTagSetPosition (int inTag, point inPoint, fixed inHeightOffset);
native void     TextTagAttachToUnit (int inTag, unit inUnit, fixed inHeightOffset);
native void     TextTagAttachToUnitPoint (int inTag, unit inUnit, string attachmentPoint, int offsetX, int offsetY);
native void     TextTagSetVelocity (int inTag, fixed inSpeed, fixed inAngle);
native void     TextTagSetAlignment (int inTag, int inHoriz, int inVert);
native void     TextTagSetTextAlignment (int inTag, int inHoriz, int inVert);
native void     TextTagSetMaxSize (int inTag, fixed inWidth, fixed inHeight);

native void     TextTagShowBackground (int inTag, bool inShow);
native void     TextTagSetBackgroundImage (int inTag, string inPath, bool inTiled);
native void     TextTagSetBackgroundBorderSize (int inTag, fixed inX, fixed inY);
native void     TextTagSetBackgroundOffset (int inTag, fixed inX, fixed inY);
native void     TextTagSetEdgeImage (int inTag, int inEdge, string inPath, int inXOffset, int inYOffset);

native void     TextTagShow (int inTag, playergroup inPlayers, bool inShow);
native bool     TextTagVisible (int inTag, int inPlayer);
native void     TextTagPause (int inTag, bool inPause);
native void     TextTagFogofWar (int inTag, bool inPause);

const int c_textTagColorText            = -1;
const int c_textTagColorTextStart       =  0;
const int c_textTagColorTextEnd         =  1;
const int c_textTagColorBackground      = -2;
const int c_textTagColorBackgroundStart =  2;
const int c_textTagColorBackgroundEnd   =  3;
const int c_textTagColorEdge            = -3;
const int c_textTagColorEdgeStart       =  4;
const int c_textTagColorEdgeEnd         =  5;

native void     TextTagSetColor (int inTag, int inType, color inColor);

const int c_textTagFadeAll          = -1;
const int c_textTagFadeText         =  0;
const int c_textTagFadeBackground   =  1;
const int c_textTagFadeEdge         =  2;

native void     TextTagSetFadedTransparency (int inTag, int inType, fixed inTransparency);

const int c_textTagTimeDuration     = 0;
const int c_textTagTimeFadeOut      = 1;
const int c_textTagTimeFadeDuration = 2; // If zero, will fade over full duration
const int c_textTagTimeCurrent      = 3;

const fixed c_textTagTimePermanent  = 0; // Use with c_textTagTimeDuration for permanent text

native void     TextTagSetTime (int inTag, int inType, fixed inVal);

//--------------------------------------------------------------------------------------------------
// Timing
//
// -- Time types --
//
// Timing functions can work with three different types of time:
//
//   Game Time - Takes into account the current game speed, and will pass more slowly at slower
//               speeds, and vice versa.
//
//   Real Time - Passes at the same rate regardless of game speed, and approximates time passing
//               in the real world for the user.  However, "real" time is still synchronous, and
//               will therefore be affected by lag due to network or system issues.
//
//   AI Time   - Passes at the same rate as game time, but may be paused/unpaused independently
//               of game time via triggers.  This is most commonly done to pause the AI while
//               an in-game cinematic sequence is run.
//
//--------------------------------------------------------------------------------------------------
const int c_timeGame    = 0;
const int c_timeReal    = 1;
const int c_timeAI      = 2;

native void     Wait (fixed inSecs, int inTimeType);

// AI Time
native void     AITimePause (bool inPause);
native bool     AITimeIsPaused ();

// Timers
const fixed c_timerDurationInfinite = -1.0;

native timer    TimerCreate ();

native void     TimerStart (timer t, fixed duration, bool periodic, int timeType);
native void     TimerRestart (timer t);
native timer    TimerLastStarted ();

native void     TimerPause (timer t, bool pause);
native bool     TimerIsPaused (timer t);

native fixed    TimerGetElapsed (timer t);
native fixed    TimerGetRemaining (timer t);
native fixed    TimerGetDuration (timer t);

// Timer events
native void TriggerAddEventTimer (trigger inTrigger, timer inTimer);
native void TriggerAddEventTimeElapsed (trigger inTrigger, fixed inTime, int inTimeType);
native void TriggerAddEventTimePeriodic (trigger inTrigger, fixed inTime, int inTimeType);

native timer    EventTimer ();

// Timer windows
//   inShowElapsed shows elapsed vs. remaining time (defaults off)
//
const int c_timerWindowNone = 0;

native int      TimerWindowCreate (timer inTimer, text inTitle, bool inShow, bool inShowElapsed);
native int      TimerWindowLastCreated ();
native void     TimerWindowDestroy (int inWindow);

native void     TimerWindowShow (int inWindow, playergroup inPlayers, bool inShow);
native bool     TimerWindowVisible (int inWindow, int inPlayer);
native void     TimerWindowSetTimer (int inWindow, timer inTimer);
native void     TimerWindowSetTitle (int inWindow, text inTitle);

// - Style
const int c_timerWindowStyleHorizontalTitleTime = 0;
const int c_timerWindowStyleHorizontalTimeTitle = 1;
const int c_timerWindowStyleVerticalTitleTime   = 2;
const int c_timerWindowStyleVerticalTimeTitle   = 3;

native void     TimerWindowSetStyle (int inWindow, int inStyle, bool inShowElapsed);
native void     TimerWindowSetGapWidth (int inWindow, int inWidth);

// - Time Format - See the TextTimeFormat function for details on the format string
native void     TimerWindowSetFormat (int inWindow, text inFormat);

const int c_timerWindowColorTitle       = 0;
const int c_timerWindowColorTime        = 1;
const int c_timerWindowColorBackground  = 2;

native void     TimerWindowSetColor (int inWindow, int inType, color inColor, fixed transparency);

//--------------------------------------------------------------------------------------------------
// Transmission
//--------------------------------------------------------------------------------------------------
const int c_invalidTransmissionId = 0;

const fixed c_transmissionTransitionDuration = 0.5;

const int c_transmissionDurationDefault = 0;
const int c_transmissionDurationAdd     = 1;
const int c_transmissionDurationSub     = 2;
const int c_transmissionDurationSet     = 3;

native transmissionsource   TransmissionSource ();
native transmissionsource   TransmissionSourceFromUnit (unit u, bool flash, bool overridePortrait, string anim);
native transmissionsource   TransmissionSourceFromUnitType (string unitType, bool overridePortrait);
native transmissionsource   TransmissionSourceFromModel (string modelLink);
native transmissionsource   TransmissionSourceFromMovie (string assetLink, bool subtitles);

native int                  TransmissionSend (
                                playergroup players,
                                transmissionsource source,
                                int targetPortrait,
                                string targetAnim,
                                soundlink soundLink,
                                text speaker,
                                text subtitle,
                                fixed duration,
                                int durationType,
                                bool waitUntilDone
                            );
native int                  TransmissionLastSent ();
native void                 TransmissionClear (int t);
native void                 TransmissionClearAll ();
native void                 TransmissionWait (int t, fixed offset);

//--------------------------------------------------------------------------------------------------
// Triggers
//
// Each trigger has one script function, registered in TriggerCreate,
// which is expected to look like:
//
//  bool TriggerFunc (bool testConditions, bool runActions) {
//      if (testConditions) {
//          if (<conditions fail>) {
//              return false;
//          }
//      }
//
//      if (!runActions) {
//          return true;
//      }
//
//      <do actions>
//      return true;
//  }
//
//--------------------------------------------------------------------------------------------------
native trigger  TriggerCreate (string inFunction);
native void     TriggerDestroy (trigger t);

native void     TriggerEnable (trigger t, bool enable);
native bool     TriggerIsEnabled (trigger t);

native void     TriggerResetCounts (trigger t);
native int      TriggerGetEvalCount (trigger t);
native int      TriggerGetExecCount (trigger t);

native bool     TriggerEvaluate (trigger t);
native void     TriggerExecute (trigger t, bool testConds, bool waitUntilDone);
native trigger  TriggerGetCurrent (); // Returns the trigger currently executing, if any
native void     TriggerStop (trigger t); // Kills all executing instances of the input trigger
native void     TriggerWaitForTrigger (trigger t, bool waitUntilDone); // Wait until the given trigger executes

native void     TriggerQueueEnter ();
native void     TriggerQueueExit ();
native bool     TriggerQueueIsEmpty ();
native void     TriggerQueuePause (bool pause);

// Options for handling the active thread when clearing the trigger queue
const int c_triggerQueueRetain  = 0; // Leave in the queue
const int c_triggerQueueRemove  = 1; // Remove from the queue, but leave it running
const int c_triggerQueueKill    = 2; // Remove from the queue and kill it immediately

native void     TriggerQueueClear (int activeOption);

native void     TriggerSkippableBegin (
                    playergroup allowedToSkip,
                    int requiredCount,
                    trigger onSkip,
                    bool testConds,
                    bool waitUntilDone
                );
native void     TriggerSkippableEnd ();

// Trigger Debugging
native void     TriggerDebugWindowOpen (bool open);
native void     TriggerDebugOutput (int type, text inText, bool includeGameUI);
native void     TriggerDebugSetTypeName (int type, text inText);
native void     TriggerDebugSetTypeColor (int type, color c);
native void     TriggerDebugSetTypeFile (int type, string file);

//--------------------------------------------------------------------------------------------------
// Units
//--------------------------------------------------------------------------------------------------

// Spend locations for simulating spend calls
const int c_spendLocationAll        = -1;
const int c_spendLocationAbility    = 0;
const int c_spendLocationBehavior   = 1;
const int c_spendLocationUnit       = 2;
const int c_spendLocationPlayer     = 3;
const int c_spendLocationGlobal     = 4;

// Unit creation styles
const int c_unitCreateConstruct         = (1 << 0);
const int c_unitCreateIgnorePlacement   = (1 << 1);

native unitgroup UnitCreate (
    int inCount,
    string inUnitType,
    int inCreateStyle,
    int inPlayer,
    point inPos,
    fixed inFacing
);

native unit UnitLastCreated ();
native unitgroup UnitLastCreatedGroup ();
native unit UnitFromId (int id);
native void UnitLoadModel (unit inUnit);
native void UnitUnloadModel (unit inUnit);
native void UnitRemove (unit inUnit);
native void UnitKill (unit inUnit);
native void UnitRevive (unit inUnit);
native bool UnitIsAlive (unit inUnit);
native bool UnitIsValid (unit inUnit);
native void UnitWaitUntilIdle (unit inUnit, bool inIdle);

native string UnitGetType (unit inUnit);

native int   UnitGetOwner (unit inUnit);
native void  UnitSetOwner (unit inUnit, int inPlayer, bool inChangeColor);

native void  UnitSetTeamColorIndex (unit inUnit, int inIndex);
native void  UnitResetTeamColorIndex (unit inUnit);

native point UnitGetPosition (unit inUnit);
native void  UnitSetPosition (unit inUnit, point inPos, bool blend);
native fixed UnitGetHeight (unit inUnit); // Absolute height in world space
native void UnitSetHeight (unit inUnit, fixed inHeight, fixed inDuration);

native fixed UnitGetFacing (unit inUnit);
native void  UnitSetFacing (unit inUnit, fixed inFacing, fixed inDuration);

native point UnitGetAttachmentPoint (unit inUnit, string attachment);

native void  UnitResetSpeed (unit inUnit);

native void UnitSetScale (unit inUnit, fixed x, fixed y, fixed z);

native void UnitPauseAll (bool inPause);

native void UnitSetCursor (unit inUnit, string cursorLink);
native void UnitSetInfoText (unit inUnit, text info, text tip, text subTip);
native void UnitClearInfoText (unit inUnit);

native void UnitForceStatusBar (unit inUnit, bool inShow);

// Unit states
const int c_unitStateBuried             = 0;  // Read-only
const int c_unitStateCloaked            = 1;  // Read-only
const int c_unitStateDetector           = 2;  // Read-only
const int c_unitStateRadar              = 3;  // Read-only
const int c_unitStateVisionSuppressed   = 4;  // Read-only
const int c_unitStateAttackSuppressed   = 5;  // Read-only
const int c_unitStateInStasis           = 6;  // Read-only
const int c_unitStateHallucination      = 7;  // Read-only
const int c_unitStateInvulnerable       = 8;
const int c_unitStatePaused             = 9;
const int c_unitStateHidden             = 10;
const int c_unitStateHighlightable      = 11;
const int c_unitStateIgnoreTerrainZ     = 12;
const int c_unitStateUnderConstruction  = 13; // Read-only
const int c_unitStateInsideTransport    = 14; // Read-only
const int c_unitStateIdle               = 15; // Read-only
const int c_unitStateFidget             = 16;
const int c_unitStateSelectable         = 17;
const int c_unitStateTargetable         = 18;
const int c_unitStateStatusBar          = 19;
const int c_unitStateTooltipable        = 20;
const int c_unitStateCursorable         = 21;
const int c_unitStateIsDead             = 22; // Read-only
const int c_unitStateIsTransport        = 23; // Read-only
const int c_unitStateMoveSuppressed     = 24;
const int c_unitStateTurnSuppressed     = 25;
const int c_unitStateHighlighted        = 26;
const int c_unitStateUsingSupply        = 27;
const int c_unitStateRevivable          = 28; // Read-only

native void UnitSetState (unit inUnit, int inState, bool inVal);
native bool UnitTestState (unit inUnit, int inState);

// Unit properties
const int c_unitPropLife            =  0;
const int c_unitPropLifePercent     =  1;
const int c_unitPropLifeMax         =  2;
const int c_unitPropLifeRegen       =  3;
const int c_unitPropEnergy          =  4;
const int c_unitPropEnergyPercent   =  5;
const int c_unitPropEnergyMax       =  6;
const int c_unitPropEnergyRegen     =  7;
const int c_unitPropShields         =  8;
const int c_unitPropShieldsPercent  =  9;
const int c_unitPropShieldsMax      = 10;
const int c_unitPropShieldsRegen    = 11;
const int c_unitPropSuppliesUsed    = 12; // Read-only
const int c_unitPropSuppliesMade    = 13; // Read-only
const int c_unitPropKills           = 14;
const int c_unitPropVitality        = 15; // Read-only
const int c_unitPropVitalityPercent = 16; // Read-only
const int c_unitPropVitalityMax     = 17; // Read-only
const int c_unitPropAcceleration    = 18; // Read-only
const int c_unitPropHeight          = 19;
const int c_unitPropMovementSpeed   = 20;
const int c_unitPropTurnRate        = 21; // Read-only
const int c_unitPropResources       = 22;
const int c_unitPropRadius          = 23; // Read-only

const bool c_unitPropCurrent = true;
const bool c_unitPropNormal  = false;

native void     UnitSetPropertyInt (unit inUnit, int inProp, int inVal);
native void     UnitSetPropertyFixed (unit inUnit, int inProp, fixed inVal);

native int      UnitGetPropertyInt (unit inUnit, int inProp, bool inCurrent);
native fixed    UnitGetPropertyFixed (unit inUnit, int inProp, bool inCurrent);

native void     UnitSetCustomValue (unit inUnit, int inIndex, fixed inVal);
native fixed    UnitGetCustomValue (unit inUnit, int inIndex);

native void     UnitAddChargeRegen (unit inUnit, string inCharge, fixed inVal);
native fixed    UnitGetChargeRegen (unit inUnit, string inCharge);
native void     UnitAddChargeUsed (unit inUnit, string inCharge, fixed inVal);
native fixed    UnitGetChargeUsed (unit inUnit, string inCharge);

native void     UnitAddCooldown (unit inUnit, string inCooldown, fixed inVal);
native fixed    UnitGetCooldown (unit inUnit, string inCooldown);

native void     UnitCreateEffectPoint (unit inUnit, string inEffect, point inTarget);
native void     UnitCreateEffectUnit (unit inUnit, string inEffect, unit inTarget);
native int      UnitValidateEffectPoint (unit inUnit, string inEffect, point inTarget);
native int      UnitValidateEffectUnit (unit inUnit, string inEffect, unit inTarget);

// Unit AI options
const int c_unitAIOptionUsable      = 0;

native void     UnitSetAIOption (unit inUnit, int inOption, bool inVal);
native bool     UnitGetAIOption (unit inUnit, int inOption);

// Unit abilities

// Ability charge types
const int c_unitAbilChargeCountMax   = 0;
const int c_unitAbilChargeCountUse   = 1;
const int c_unitAbilChargeCountLeft  = 2;
const int c_unitAbilChargeRegenMax   = 3;
const int c_unitAbilChargeRegenLeft  = 4;

native void     UnitAbilitySpend (unit inUnit, abilcmd inAbilCmd, int inLocation);
native void     UnitAbilityReset (unit inUnit, abilcmd inAbilCmd, int inLocation);
native fixed    UnitAbilityChargeInfo (unit inUnit, abilcmd inAbilCmd, int inType);
native int      UnitAbilityCount (unit inUnit);
native bool     UnitAbilityExists (unit inUnit, string inAbil);
native void     UnitAbilityEnable (unit inUnit, string inAbil, bool inEnable);
native void     UnitAbilityShow (unit inUnit, string inAbil, bool inShow);
native string   UnitAbilityGet (unit inUnit, int inIndex);
native bool     UnitAbilityCheck (unit inUnit, string inAbil, bool inEnabled);

native void     UnitAbilityChangeLevel (unit inUnit, string inAbil, int inLevel);
native int      UnitAbilityGetLevel (unit inUnit, string inAbil);
native int      UnitAbilityMaxLevel (unit inUnit, string inAbil);

native void     UnitAbilityAddChargeRegen (unit inUnit, string inAbil, string inCharge, fixed inVal);
native fixed    UnitAbilityGetChargeRegen (unit inUnit, string inAbil, string inCharge);
native void     UnitAbilityAddChargeUsed (unit inUnit, string inAbil, string inCharge, fixed inVal);
native fixed    UnitAbilityGetChargeUsed (unit inUnit, string inAbil, string inCharge);

native void     UnitAbilityAddCooldown (unit inUnit, string inAbil, string inCooldown, fixed inVal);
native fixed    UnitAbilityGetCooldown (unit inUnit, string inAbil, string inCooldown);

// Ability command states
const int c_cmdStateHidden          = (1 << 0);
const int c_cmdStateExecuting       = (1 << 1);
const int c_cmdStateCanAutoCast     = (1 << 2);
const int c_cmdStateIsAutoCast      = (1 << 3);
const int c_cmdStateMaxCharges      = (1 << 4);
const int c_cmdStateHasCharges      = (1 << 5);
const int c_cmdStateDisabled        = (1 << 6);
const int c_cmdStateCooldown        = (1 << 7);
const int c_cmdStateNoLife          = (1 << 8);
const int c_cmdStateNoShields       = (1 << 9);
const int c_cmdStateNoEnergy        = (1 << 10);
const int c_cmdStateNoMinerals      = (1 << 11);
const int c_cmdStateNoVespene       = (1 << 12);
const int c_cmdStateNoTerrazine     = (1 << 13);
const int c_cmdStateNoCustom        = (1 << 14);
const int c_cmdStateNoFood          = (1 << 15);
const int c_cmdStateCantSpend       = (1 << 16);

native bool     UnitCheckAbilCmdState (unit inUnit, abilcmd inAbilCmd, int inState);

const int c_unitCargoUnitCount      = 0;    // Number of units contained as cargo
const int c_unitCargoSpaceTotal     = 1;    // Total cargo space
const int c_unitCargoSpaceUsed      = 2;    // Cargo space in use
const int c_unitCargoSpaceFree      = 3;    // Cargo space available (total - used)
const int c_unitCargoSizeAsCargo    = 4;    // Space used by this unit as cargo
const int c_unitCargoSizeMax        = 5;    // Maximum cargo size per unit this unit can contain
const int c_unitCargoPosition       = 6;    // Unit's index in the transport list

native void         UnitCargoCreate (unit inUnit, string inId, int inCount);
native unit         UnitCargoLastCreated ();
native unitgroup    UnitCargoLastCreatedGroup ();
native unit         UnitCargo (unit inUnit, int inIndex);
native unitgroup    UnitCargoGroup (unit inUnit);
native int          UnitCargoValue (unit inUnit, int inValue);
native unit         UnitTransport (unit inUnit);

native unit     UnitAgent (unit inUnit, int inPlayer);

native void     UnitMagazineArm (unit inUnit, abilcmd inAbilCmd, int inCount);
native int      UnitMagazineCount (unit inUnit, string inAbil);

const int c_unitQueueTimeElapsed    = 0;
const int c_unitQueueTimeRemaining  = 1;
const int c_unitQueueTimeTotal      = 2;

const int c_unitQueuePropertyAvailable  = 0;
const int c_unitQueuePropertyTotal      = 1;
const int c_unitQueuePropertyUsed       = 2;
const int c_unitQueuePropertyCount      = 3;

native int      UnitQueueItemCount (unit inUnit, int inSlot);
native string   UnitQueueItemGet (unit inUnit, int inItem, int inSlot);
native fixed    UnitQueueItemTime (unit inUnit, int inTimeType, int inItem);
native bool     UnitQueueItemTypeCheck (unit inUnit, int inItem, int inType);
native int      UnitQueueGetProperty (unit inUnit, int inProp);

native bool     UnitMoverExists (unit inUnit, string inMover);
native bool     UnitTestPlane (unit inUnit, int inPlane);

// Unit progress
const int c_unitProgressTypeConstruct       = 0;
const int c_unitProgressTypeTrain           = 1;
const int c_unitProgressTypeResearch        = 2;
const int c_unitProgressTypeArmMagazine     = 3;
const int c_unitProgressTypeSpecialize      = 4;
const int c_unitProgressTypeRevive          = 5;
const int c_unitProgressTypeLearn           = 6;

const int c_unitProgressStageStart          = 0;
const int c_unitProgressStageCancel         = 1;
const int c_unitProgressStageComplete       = 2;
const int c_unitProgressStagePause          = 3;
const int c_unitProgressStageResume         = 4;

const int c_unitProgressStateEmpty          = 0;
const int c_unitProgressStateActive         = 1;
const int c_unitProgressStatePaused         = 2;

native fixed    UnitGetProgressComplete (unit inUnit, int inSlot);
native void     UnitSetProgressComplete (unit inUnit, int inSlot, int inPercent);
native void     UnitSetProgressStage (unit inUnit, int inSlot, int inStage);
native bool     UnitCheckProgressState (unit inUnit, int inSlot, int inState);

// Unit behaviors

// Behavior categories
const int c_unitBehaviorFlagPermanent           = 0;
const int c_unitBehaviorFlagRestorable          = 1;
const int c_unitBehaviorFlagTemporary           = 2;
// Behavior buff flags
const int c_unitBehaviorFlagChanneled           = 3;
const int c_unitBehaviorFlagChanneling          = 4;
const int c_unitBehaviorFlagCountdown           = 5;
const int c_unitBehaviorFlagExtend              = 6;
const int c_unitBehaviorFlagDisableBuilding     = 7;
const int c_unitBehaviorFlagRemoveDamageResponseExhausted = 8;
// Behavior info flags
const int c_unitBehaviorFlagHidden              = 9;
// Behavior count
const int c_unitBehaviorCountAll                = -1;

native void     UnitBehaviorAdd (unit inUnit, string inBehavior, unit inCaster, int inCount);
native void     UnitBehaviorAddPlayer (unit inUnit, string inBehavior, int inPlayer, int inCount);
native int      UnitBehaviorCountAll (unit inUnit);
native int      UnitBehaviorCount (unit inUnit, string inBehavior);
native fixed    UnitBehaviorDuration (unit inUnit, string inBehavior);
native void     UnitBehaviorSetDuration (unit inUnit, string inBehavior, fixed inDuration);
native bool     UnitBehaviorEnabled (unit inUnit, string inBehavior);
native string   UnitBehaviorGet (unit inUnit, int inIndex);
native bool     UnitHasBehavior (unit inUnit, string inBehavior);
native void     UnitBehaviorRemove (unit inUnit, string inBehavior, int inCount);
native void     UnitBehaviorRemovePlayer (unit inUnit, string inBehavior, int inPlayer, int inCount);
native void     UnitBehaviorTransfer (unit inSource, unit inDest, string inBehavior, int inCount);
native bool     UnitBehaviorHasFlag (string inBehavior, int inCategory);
native void     UnitBehaviorRemoveCategory (unit inUnit, int inCategory);

native void     UnitBehaviorAddChargeRegen (unit inUnit, string inBehavior, string inCharge, fixed inVal);
native fixed    UnitBehaviorGetChargeRegen (unit inUnit, string inBehavior, string inCharge);
native void     UnitBehaviorAddChargeUsed (unit inUnit, string inBehavior, string inCharge, fixed inVal);
native fixed    UnitBehaviorGetChargeUsed (unit inUnit, string inBehavior, string inCharge);

native void     UnitBehaviorAddCooldown (unit inUnit, string inBehavior, string inCooldown, fixed inVal);
native fixed    UnitBehaviorGetCooldown (unit inUnit, string inBehavior, string inCooldown);

native void     UnitXPGainEnable (unit inUnit, string inBehavior, bool inEnable);

// Unit markers
native marker   UnitMarker (unit inUnit, int inIndex);
native void     UnitMarkerAdd (unit inUnit, marker inMarker);
native int      UnitMarkerCount (unit inUnit, marker inMarker);
native void     UnitMarkerRemove (unit inUnit, marker inMarker);

// Unit orders
const int c_orderQueueReplace       = 0;
const int c_orderQueueAddToEnd      = 1;
const int c_orderQueueAddToFront    = 2;

native order    UnitOrder (unit inUnit, int inIndex);
native int      UnitOrderCount (unit inUnit);
native bool     UnitOrderHasAbil (unit inUnit, string abilLink);
native bool     UnitOrderIsValid (unit inUnit, order inOrder);

native bool     UnitIsHarvesting (unit inUnit, int inResource);

native bool     UnitIssueOrder (unit inUnit, order inOrder, int inQueueType);

// Unit rallying
native int      UnitRallyPoint (unit inUnit, unit inUser);
native int      UnitRallyPointCount (unit inUnit);
native int      UnitRallyPointTargetCount (unit inUnit, int inPoint);
native point    UnitRallyPointTargetPoint (unit inUnit, int inPoint, int inTarget);
native unit     UnitRallyPointTargetUnit (unit inUnit, int inPoint, int inTarget);

// Unit tech
native int UnitTechTreeBehaviorCount (unit inUnit, string behaviorType, int countType);
native int UnitTechTreeUnitCount (unit inUnit, string unitType, int countType);
native int UnitTechTreeUpgradeCount (unit inUnit, string upgradeType, int countType);

// Unit inventory
const int c_unitInventoryCountCarried    = 0;
const int c_unitInventoryCountEmpty      = 1;
const int c_unitInventoryCountTotal      = 2;

native int       UnitInventoryCount (unit inUnit, int inCountType);
native unit      UnitInventoryItem (unit inUnit, int inIndex);
native unitgroup UnitInventoryGroup (unit inUnit);
native unit      UnitInventoryCreate (unit inUnit, string itemType);
native unit      UnitInventoryLastCreated ();

// Unit weapons
native void         UnitWeaponAdd (unit inUnit, string inWeapon, string inTurret);
native int          UnitWeaponCount (unit inUnit);
native bool         UnitWeaponCheck (unit inUnit, int inIndex, int inTarget);
native string       UnitWeaponGet (unit inUnit, int inIndex);
native bool         UnitWeaponIsEnabled (unit inUnit, int inIndex);
native fixed        UnitWeaponPeriod (unit inUnit, int inIndex);
native void         UnitWeaponRemove (unit inUnit, string inWeapon);
native bool         UnitWeaponsPlaneTest (unit inUnit, int inPlane);

// Unit experience
native fixed    UnitXPTotal (unit inUnit);
native int      UnitLevel (unit inUnit);

// Unit Costs
const int c_unitCostMinerals            = 0;
const int c_unitCostVespene             = 1;
const int c_unitCostTerrazine           = 2;
const int c_unitCostCustomResource      = 3;
const int c_unitCostSumMineralsVespene  = 4;

native string UnitTypeFromString (string inString);
native text UnitTypeGetName (string inUnitType);
native fixed UnitTypeGetProperty (string inUnitType, int inProp);
native int UnitTypeGetCost (string inUnitType, int inCostType);
native bool UnitTypeTestFlag (string inUnitType, int inFlag);
native bool UnitTypeTestAttribute (string inUnitType, int inAttribute);
native bool UnitTypeIsAffectedByUpgrade (string inUnitType, string inUpgrade);

native void UnitTypeAnimationLoad (string inUnitType, string animPath);
native void UnitTypeAnimationUnload (string inUnitType, string animPath);

// Unit events
// Note: Use a null unit for "any unit" events

// Damage fatal option
const int c_unitDamageEither    = 0;
const int c_unitDamageFatal     = 1;
const int c_unitDamageNonFatal  = 2;

// Damage type
const int c_unitDamageTypeAny       = -1;
const int c_unitDamageTypeSpell     = 0;
const int c_unitDamageTypeMelee     = 1;
const int c_unitDamageTypeRanged    = 2;
const int c_unitDamageTypeSplash    = 3;

native void     TriggerAddEventUnitCreated (trigger t, unitref u, string creatorAbil, string creatorBehavior);
native void     TriggerAddEventUnitRemoved (trigger t, unitref u);
native void     TriggerAddEventUnitDied (trigger t, unitref u);
native void     TriggerAddEventUnitGainExperience (trigger t, unitref u);
native void     TriggerAddEventUnitGainLevel (trigger t, unitref u);
native void     TriggerAddEventUnitAcquiredTarget (trigger t, unitref u);
native void     TriggerAddEventUnitStartedAttack (trigger t, unitref u);
native void     TriggerAddEventUnitAttacked (trigger t, unitref u);
native void     TriggerAddEventUnitAttributeChange (trigger t, unitref u);
native void     TriggerAddEventUnitDamaged (trigger inTrigger, unitref inUnit, int inDamageType, int inDamageFatal, string inEffect);
native void     TriggerAddEventUnitBecomesIdle (trigger t, unitref u, bool idle);

native unit     EventUnit ();
native unit     EventUnitTarget ();

// Responses
// - c_unitEventCreated
native unit     EventUnitCreatedUnit ();
native string   EventUnitCreatedAbil ();
native string   EventUnitCreatedBehavior ();

// - c_unitEventDamaged
// - c_unitEventDied
native fixed    EventUnitDamageAmount ();
native unit     EventUnitDamageSourceUnit ();
native int      EventUnitDamageSourcePlayer ();
native point    EventUnitDamageSourcePoint ();
native bool     EventUnitDamageDeathCheck (int inType);
native string   EventUnitDamageEffect ();

// - c_unitEventGainExperience
// - c_unitEventGainLevel
native string   EventUnitBehavior ();
native fixed    EventUnitXPDelta ();
native int      EventUnitAttributePoints ();

// Property change events
native void     TriggerAddEventUnitProperty (trigger t, unitref u, int prop);

// State events
//   region/range events    - true = entered,       false = exited
//   cargo events           - true = loaded,        false = unloaded
//   selection events       - true = selected,      false = deselected
//   highlight events       - true = highlighted,   false = unhighlighted
//
native void     TriggerAddEventUnitRegion (trigger t, unitref u, region r, bool state);
native void     TriggerAddEventUnitRange (trigger t, unitref u, unit fromUnit, fixed range, bool state);
native void     TriggerAddEventUnitRangePoint (trigger t, unitref u, point p, fixed distance, bool state);
native void     TriggerAddEventUnitCargo (trigger t, unitref u, bool state);

native region   EventUnitRegion ();
native unit     EventUnitCargo ();

// Player events
// Note: EventPlayer (defined in the Players section) is valid for these events
//
native void     TriggerAddEventUnitSelected (trigger t, unitref u, int player, bool state);
native void     TriggerAddEventUnitClick (trigger t, unitref u, int player);
native void     TriggerAddEventUnitHighlight (trigger t, unitref u, int player, bool state);

// Order events
native void     TriggerAddEventUnitOrder (trigger t, unitref u, abilcmd a);
native order    EventUnitOrder ();

// Ability events
const int c_unitAbilStageComplete           = -6;
const int c_unitAbilStagePreempt            = -5;
const int c_unitAbilStageCancel             = -4;
const int c_unitAbilStageExecute            = -3;
const int c_unitAbilStageQueue              = -2;
const int c_unitAbilStageAll                = -1;

native void     TriggerAddEventUnitAbility (trigger t, unitref u, abilcmd a, int stage, bool includeSharedAbils);

native abilcmd  EventUnitAbility ();
native int      EventUnitAbilityStage ();
native point    EventUnitTargetPoint ();
native unit     EventUnitTargetUnit ();

// Progress events
native void     TriggerAddEventUnitConstructProgress (trigger t, unitref u, int stage);
native void     TriggerAddEventUnitTrainProgress (trigger t, unitref u, int stage);
native void     TriggerAddEventUnitResearchProgress (trigger t, unitref u, int stage);
native void     TriggerAddEventUnitArmMagazineProgress (trigger t, unitref u, int stage);
native void     TriggerAddEventUnitSpecializeProgress (trigger t, unitref u, int stage);

native string   EventUnitProgressObjectType (); // The type of object being trained/researched/etc
native unit     EventUnitProgressUnit ();       // The unit being trained, if any

// Powerup
native void     TriggerAddEventUnitPowerup (trigger t, unitref u);
native unit     EventUnitPowerupUnit ();

// Revive
native void     TriggerAddEventUnitRevive (trigger t, unitref u);

//--------------------------------------------------------------------------------------------------
// Unit Filters
//--------------------------------------------------------------------------------------------------
// Construct a unitfilter based on bit flags corresponding to c_targetFilter<*> constants
// Note: If the target flag is >= than 32, it should go in the second integer.  For example:
//
//       UnitFilter (
//          (1 << c_targetFilterGround),                // Require ground
//          0,
//          0,
//          (1 << (c_targetFilterInvulnerable - 32))    // Exclude invulnerable
//       );
//
native unitfilter UnitFilter (int inRequired1, int inRequired2, int inExcluded1, int inExcluded2);
native unitfilter UnitFilterStr (string filters);

// Set/Get individual filter states by index
//
const int c_unitFilterAllowed   = 0;    // Neither required nor excluded
const int c_unitFilterRequired  = 1;
const int c_unitFilterExcluded  = 2;

native void UnitFilterSetState (unitfilter inFilter, int inType, int inState);
native int  UnitFilterGetState (unitfilter inFilter, int inType);
native bool UnitFilterMatch (unit inUnit, int inPlayer, unitfilter inFilter);

//--------------------------------------------------------------------------------------------------
// Unit Groups
//--------------------------------------------------------------------------------------------------
const int c_unitAllianceAny                = 0; // Any unit
const int c_unitAllianceAlly               = 1; // Units owned by allied players
const int c_unitAllianceEnemy              = 2; // Units owned by enemy players
const int c_unitAllianceAllyExcludeSelf    = 3; // Units owned by allied players not owned by player
const int c_unitAllianceAnyExcludeSelf     = 4; // Any unit not owned by player

const int c_noMaxCount = 0;

native unitgroup UnitGroupEmpty ();
native unitgroup UnitGroupCopy (unitgroup inGroup);
native unitgroup UnitGroupFromId (int id);
native unitgroup UnitGroup (string type, int player, region r, unitfilter filter, int maxCount);
native unitgroup UnitGroupAlliance (int player, int alliance, region r, unitfilter filter, int maxCount);
native unitgroup UnitGroupFilter (string type, int player, unitgroup g, unitfilter filter, int maxCount);
native unitgroup UnitGroupFilterAlliance (unitgroup g, int player, int alliance, int maxCount);
native unitgroup UnitGroupFilterPlane (unitgroup g, int plane, int maxCount);
native unitgroup UnitGroupFilterPlayer (unitgroup g, int player, int maxCount);
native unitgroup UnitGroupFilterRegion (unitgroup g, region r, int maxCount);
native unitgroup UnitGroupFilterThreat (unitgroup g, unit u, string alternateType, int maxCount);
native unitgroup UnitGroupIdle (int player, bool workerOnly);

native void UnitGroupClear (unitgroup inGroup);
native void UnitGroupAdd (unitgroup inGroup, unit inUnit);
native void UnitGroupRemove (unitgroup inGroup, unit inUnit);

native bool UnitGroupIssueOrder (unitgroup inGroup, order inOrder, int inQueueType);
native void UnitGroupWaitUntilIdle (unitgroup inGroup, int inCount, bool inIdle);

const int c_unitCountAll    = 0;
const int c_unitCountAlive  = 1;
const int c_unitCountDead   = 2;

native int  UnitGroupCount (unitgroup inGroup, int inType);
native unit UnitGroupUnit (unitgroup inGroup, int inIndex); // index is one-based
native unit UnitGroupRandomUnit (unitgroup inGroup, int inType);
native bool UnitGroupHasUnit (unitgroup inGroup, unit inUnit);
native bool UnitGroupTestPlane (unitgroup inGroup, int inPlane);
native unit UnitGroupNearestUnit (unitgroup inGroup, point inPoint);

//--------------------------------------------------------------------------------------------------
// Unit Reference
// - A unitref may refer to a unit explicitly or through a unit variable
//--------------------------------------------------------------------------------------------------
native unitref UnitRefFromUnit (unit u);
native unitref UnitRefFromVariable (string v);

native unit UnitRefToUnit (unitref r);

//--------------------------------------------------------------------------------------------------
// Unit Selection
//
// Notes:
// - UnitSelect and UnitGroupSelect set the local selection state,
//   which must be then transmitted across the network.
//
//   However, UnitIsSelected and UnitGroupSelected query the synchronous selection state,
//   which means that if you set the selection, then immediately query it, the results will
//   not match.
//
// - Unit selection state cannot be set during map initialization events.
//
//--------------------------------------------------------------------------------------------------
native void         UnitSelect (unit inUnit, int inPlayer, bool inSelect);
native void         UnitGroupSelect (unitgroup inGroup, int inPlayer, bool inSelect);
native void         UnitClearSelection (int inPlayer);

native bool         UnitIsSelected (unit inUnit, int inPlayer);
native unitgroup    UnitGroupSelected (int inPlayer);

native void         UnitFlashSelection (unit inUnit, fixed inPeriod);

// Control groups
native void         UnitControlGroupAddUnit (int inPlayer, int inGroup, unit inUnit);
native void         UnitControlGroupAddUnits (int inPlayer, int inGroup, unitgroup inUnits);
native void         UnitControlGroupRemoveUnit (int inPlayer, int inGroup, unit inUnit);
native void         UnitControlGroupRemoveUnits (int inPlayer, int inGroup, unitgroup inUnits);
native void         UnitControlGroupClear (int inPlayer, int inGroup);

//--------------------------------------------------------------------------------------------------
// User Interface
//--------------------------------------------------------------------------------------------------
const fixed c_transitionDurationImmediate   = 0.0;
const fixed c_transitionDurationDefault     = -1.0;

const int   c_messageAreaAll                = 0;
const int   c_messageAreaChat               = 1;
const int   c_messageAreaObjective          = 2;
const int   c_messageAreaDirective          = 3;
const int   c_messageAreaError              = 4;
const int   c_messageAreaSubtitle           = 5;
const int   c_messageAreaCinematic          = 6;
const int   c_messageAreaDebug              = 7;
const int   c_messageAreaWarning            = 8;

const int   c_uiModeFullscreen              = 0;
const int   c_uiModeLetterboxed             = 1;
const int   c_uiModeConsole                 = 2;

const int   c_mouseButtonNone               = 0;
const int   c_mouseButtonLeft               = 1;
const int   c_mouseButtonMiddle             = 2;
const int   c_mouseButtonRight              = 3;
const int   c_mouseButtonXButton1           = 4;
const int   c_mouseButtonXButton2           = 5;

const int   c_scaleNormal                   = 0;
const int   c_scaleAspect                   = 1;
const int   c_scaleStretch                  = 2;

const int c_keyNone                  = -1;
const int c_keyShift                 = 0;
const int c_keyControl               = 1;
const int c_keyAlt                   = 2;
const int c_key0                     = 3;
const int c_key1                     = 4;
const int c_key2                     = 5;
const int c_key3                     = 6;
const int c_key4                     = 7;
const int c_key5                     = 8;
const int c_key6                     = 9;
const int c_key7                     = 10;
const int c_key8                     = 11;
const int c_key9                     = 12;
const int c_keyA                     = 13;
const int c_keyB                     = 14;
const int c_keyC                     = 15;
const int c_keyD                     = 16;
const int c_keyE                     = 17;
const int c_keyF                     = 18;
const int c_keyG                     = 19;
const int c_keyH                     = 20;
const int c_keyI                     = 21;
const int c_keyJ                     = 22;
const int c_keyK                     = 23;
const int c_keyL                     = 24;
const int c_keyM                     = 25;
const int c_keyN                     = 26;
const int c_keyO                     = 27;
const int c_keyP                     = 28;
const int c_keyQ                     = 29;
const int c_keyR                     = 30;
const int c_keyS                     = 31;
const int c_keyT                     = 32;
const int c_keyU                     = 33;
const int c_keyV                     = 34;
const int c_keyW                     = 35;
const int c_keyX                     = 36;
const int c_keyY                     = 37;
const int c_keyZ                     = 38;
const int c_keySpace                 = 39;
const int c_keyGrave                 = 40;
const int c_keyNumPad0               = 41;
const int c_keyNumPad1               = 42;
const int c_keyNumPad2               = 43;
const int c_keyNumPad3               = 44;
const int c_keyNumPad4               = 45;
const int c_keyNumPad5               = 46;
const int c_keyNumPad6               = 47;
const int c_keyNumPad7               = 48;
const int c_keyNumPad8               = 49;
const int c_keyNumPad9               = 50;
const int c_keyNumPadPlus            = 51;
const int c_keyNumPadMinus           = 52;
const int c_keyNumPadMultiply        = 53;
const int c_keyNumPadDivide          = 54;
const int c_keyNumPadDecimal         = 55;
const int c_keyEquals                = 56;
const int c_keyMinus                 = 57;
const int c_keyBracketOpen           = 58;
const int c_keyBracketClose          = 59;
const int c_keyBackSlash             = 60;
const int c_keySemiColon             = 61;
const int c_keyApostrophe            = 62;
const int c_keyComma                 = 63;
const int c_keyPeriod                = 64;
const int c_keySlash                 = 65;
const int c_keyEscape                = 66;
const int c_keyEnter                 = 67;
const int c_keyBackSpace             = 68;
const int c_keyTab                   = 69;
const int c_keyLeft                  = 70;
const int c_keyUp                    = 71;
const int c_keyRight                 = 72;
const int c_keyDown                  = 73;
const int c_keyInsert                = 74;
const int c_keyDelete                = 75;
const int c_keyHome                  = 76;
const int c_keyEnd                   = 77;
const int c_keyPageUp                = 78;
const int c_keyPageDown              = 79;
const int c_keyCapsLock              = 80;
const int c_keyNumLock               = 81;
const int c_keyScrollLock            = 82;
const int c_keyPause                 = 83;
const int c_keyPrintScreen           = 84;
const int c_keyNextTrack             = 85;
const int c_keyPrevTrack             = 86;
const int c_keyF1                    = 87;
const int c_keyF2                    = 88;
const int c_keyF3                    = 89;
const int c_keyF4                    = 90;
const int c_keyF5                    = 91;
const int c_keyF6                    = 92;
const int c_keyF7                    = 93;
const int c_keyF8                    = 94;
const int c_keyF9                    = 95;
const int c_keyF10                   = 96;
const int c_keyF11                   = 97;
const int c_keyF12                   = 98;

const int c_keyModifierStateIgnore  = 0;
const int c_keyModifierStateRequire = 1;
const int c_keyModifierStateExclude = 2;

// Types of custom dialogs
const int c_customDialogTypeMessage = 0;
const int c_customDialogTypeQuery    = 1;

// Results of custom dialogs
const int c_customDialogResultAny = 0;
const int c_customDialogResultYes = 1;
const int c_customDialogResultNo  = 2;

// Frames that can be synchronously shown / hidden
const int c_syncFrameTypeMenuBar              = 0;
const int c_syncFrameTypeCashPanel            = 1;
const int c_syncFrameTypeTipAlertPanel        = 2;
const int c_syncFrameTypeObjectivePanel       = 3;
const int c_syncFrameTypeCharacterSheetButton = 4;
const int c_syncFrameTypeSupply               = 5;
const int c_syncFrameTypeResourcePanel        = 6;
const int c_syncFrameTypeRoomPanel            = 7;
const int c_syncFrameTypePlanetPanel          = 8;
const int c_syncFrameTypeMercenaryPanel       = 9;
const int c_syncFrameTypeResearchPanel        = 10;
const int c_syncFrameTypePurchasePanel        = 11;
const int c_syncFrameTypeVictoryPanel         = 12;
const int c_syncFrameTypeBattleReportPanel    = 13;
const int c_syncFrameTypeAlertPanel           = 14;
const int c_syncFrameTypeHeroPanel            = 15;
const int c_syncFrameTypeErrorDisplayPanel    = 16;
const int c_syncFrameTypeCreditsPanel          = 17;
const int c_syncFrameTypeTechTreePanel        = 18;
const int c_syncFrameTypeTechGlossaryPanel    = 19;

// Game Menu Dialog Items
const int c_gameMenuDialogItemAny                     = -1;

// Buttons that can be renamed
const int c_gameMenuDialogAbortButton                 = 0;
const int c_gameMenuDialogGenericButton1              = 1;
const int c_gameMenuDialogGenericButton2              = 2;
const int c_gameMenuDialogGenericButton3              = 3;
const int c_gameMenuDialogGenericButton4              = 4;
const int c_gameMenuDialogGenericButton5              = 5;
const int c_gameMenuDialogGenericButton6              = 6;
const int c_gameMenuDialogGenericButton7              = 7;
const int c_gameMenuDialogGenericButton8              = 8;
const int c_gameMenuDialogGenericButton9              = 9;
const int c_gameMenuDialogGenericButton10             = 10;

// Buttons that cannot be renamed
const int c_gameMenuDialogOptionsButton               = 11;
const int c_gameMenuDialogLoadButton                  = 12;
const int c_gameMenuDialogSaveButton                  = 13;
const int c_gameMenuDialogPauseButton                 = 14;
const int c_gameMenuDialogRestartButton               = 15;
const int c_gameMenuDialogReturnToGameplayButton      = 16;
const int c_gameMenuDialogRestartDifficultyPulldown   = 17;
const int c_gameMenuDialogQuitButton                  = 18;

// UI command allowables
const int c_uiCommandAllowButtons       = 0;
const int c_uiCommandAllowHotkeys       = 1;
const int c_uiCommandAllowSmartClick    = 2;
const int c_uiCommandAllowModifiers     = 3;
const int c_uiCommandAllowInfoPanel     = 4;
const int c_uiCommandAllowMinimap       = 5;
const int c_uiCommandAllowPings         = 6;

native void         UISetMode (playergroup players, int mode, fixed duration);
native void         UISetWorldVisible (playergroup players, bool isVisible);
native void         UISetCursorVisible (playergroup players, bool isCursorVisible);
native void         UISetCursorAutoHide (playergroup players, bool autoHide, fixed delay);

native void         UIDisplayMessage (playergroup players, int messageArea, text messageText);
native void         UIClearMessages (playergroup players, int messageArea);

native void         UIShowCinematicText (playergroup inPlayers, text inMessageText, fixed inTimeBetweenCharacters, fixed inMaxTime, soundlink inSoundLink);
native void         UIHideCinematicText (playergroup inPlayers);

native void         UIShowTextCrawl (playergroup inPlayers, text inTitle, text inText, fixed inMaxTime, soundlink inBirthSoundLink, soundlink inTypeSoundLink);
native void         UIHideTextCrawl (playergroup inPlayers);

native void         UIUnitColorStyleOverride (playergroup inPlayers, int style);
native void         UIUnitColorStyleClearOverride (playergroup inPlayers);

native void         UISetNextLoadingScreen (string imagePath, text title, text subtitle, text body, text  help, bool waitForInput);
native void         UISetNextLoadingScreenImageScale (int imageScale);
native void         UISetNextLoadingScreenTextPosition (int anchor, int offsetX, int offsetY, int width, int height);

native void         UISetRestartLoadingScreen (text  help);

// Alerts (message display and recall via spacebar to a given point or unit)
// - icon may be null for the default icon
//
native void         UIAlert (string alert, int player, text message, string icon);
native void         UIAlertPoint (string alert, int player, text message, string icon, point p);
native void         UIAlertUnit (string alert, int player, text message, string icon, unit u);
native void         UIAlertClear (int player);

native void            UISetFrameVisible (playergroup inPlayers, int inFrameType, bool inVisible);
native bool            UIFrameVisible (int inPlayer, int inFrameType);

native void            UISetGameMenuItemVisible (playergroup inPlayers, int inMenuItemType, bool inVisible);
native bool            UIGameMenuItemVisible (int inPlayer, int inMenuItemType);
native void            UISetGameMenuItemText (playergroup inPlayers, int inMenuItemType, text inText);

native void         UIClearCustomMenuItemList (playergroup inPlayers);
native void            UISetCustomMenuItemVisible (playergroup inPlayers, int inMenuItemType, bool inVisible);
native bool            UICustomMenuItemVisible (int inPlayer, int inMenuItemType);
native void            UISetCustomMenuItemText (playergroup inPlayers, int inMenuItemType, text inText);
native void            UIShowCustomMenu (playergroup inPlayers, text inTitleText);

native void            UIShowCustomDialog (playergroup inPlayers, int inType, text inTitle, text inText, bool pause);

native void         UISetResourceTradeCountdownTime (int time);

native void         UISetButtonHighlighted (playergroup inPlayers, abilcmd inAbilCmd, bool inHighlight);
native void         UISetMiniMapBackGroundColor (color inColor);

native void         UISetCommandAllowed (playergroup players, int option, bool allow);
native void         UISetCommandDisallowedMessage (playergroup players, text message);

// User Interface events
native void         TriggerAddEventAbortMission (trigger t, int player);
native void         TriggerAddEventCustomDialogDismissed (trigger t, int player, int result);
native void         TriggerAddEventGameMenuItemSelected (trigger t, int player, int gameMenuIndex);
native void         TriggerAddEventMouseClicked (trigger t, int player, int mouseButton, bool down);
native void         TriggerAddEventKeyPressed (trigger t, int player, int key, bool down, int s, int c, int a);
native void         TriggerAddEventButtonPressed (trigger t, int player, string button);
native void         TriggerAddEventGameCreditsFinished (trigger t, int player);

native int          EventCustomDialogResult ();
native int          EventGameMenuItemSelected ();
native int          EventMouseClickedButton ();
native int          EventMouseClickedPosXUI ();
native int          EventMouseClickedPosYUI ();
native fixed        EventMouseClickedPosXWorld ();
native fixed        EventMouseClickedPosYWorld ();
native fixed        EventMouseClickedPosZWorld ();

native int          EventKeyPressed ();
native bool         EventKeyShift ();
native bool         EventKeyControl ();
native bool         EventKeyAlt ();

native string       EventButtonPressed ();

//--------------------------------------------------------------------------------------------------
// Visibility
//--------------------------------------------------------------------------------------------------
const int c_visTypeMask     = 0;
const int c_visTypeFog      = 1;

native void     VisEnable (int visType, bool enable);
native bool     VisIsEnabled (int visType);

native void     VisExploreArea (int player, region area, bool explored, bool checkCliffLevel);
native void     VisRevealArea (int player, region area, fixed duration, bool checkCliffLevel); // Zero duration means permanent

native revealer VisRevealerCreate (int player, region area);
native revealer VisRevealerLastCreated ();
native void     VisRevealerDestroy (revealer r);
native void     VisRevealerEnable (revealer r, bool enable);
native void     VisRevealerUpdate (revealer r); // Update revealer after region has changed


include "TriggerLibs/AI"
include "TriggerLibs/CampaignAI"
include "TriggerLibs/MeleeAI"
include "TriggerLibs/TacticalAI"

// $AS - Trigger Editor(Bug 33070): In order for line numbers to be correct you need a newline at the end of this file
